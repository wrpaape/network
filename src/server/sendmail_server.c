#include "network/network_utils.h"	/* network utils */

#ifndef SENDMAIL_PATH
#define SENDMAIL_PATH "/usr/sbin/sendmail"
#endif /* ifdef SENDMAIL_PATH */

#define DO_DEBUG		1
#define ANNOUNCE_CLIENT		1
#define REQUEST_BUFFER_SIZE	4096
#define SERVER_PORT		80
#define LISTEN_QUEUE		128
#define TOKEN			"\"dummy_token\""


#if DO_DEBUG
#	define DEBUG(...)						\
printf("(proccess: %d)\t",						\
       (int) getpid());							\
printf(__VA_ARGS__);							\
fflush(stdout)
#else
#	define DEBUG(...)
#endif /* if DO_DEBUG */

struct KeyFinder {
	const unsigned char *key;
	unsigned int length;
	unsigned int skip[UCHAR_MAX + 1];
};

#define KEY_FINDER_INIT(KEY) {						\
	.key    = (const unsigned char *) ("\"" KEY "\":"),		\
	.length = sizeof(KEY) + 2,					\
	.skip   = {							\
		[0 ... UCHAR_MAX] = sizeof(KEY) + 2			\
	}								\
}

static struct KeyFinder find_token  = KEY_FINDER_INIT("token");
static struct KeyFinder find_email  = KEY_FINDER_INIT("email");
static struct KeyFinder find_sender = KEY_FINDER_INIT("sender");
static struct KeyFinder find_room   = KEY_FINDER_INIT("room");

static struct String token;
static struct String email;
static struct String sender;
static struct String room;

static inline void
key_finder_init_skip(struct KeyFinder *const restrict finder)
{
	const unsigned char *const restrict key = finder->key;
	const unsigned int i_last		= finder->length - 1;
	unsigned int *const restrict skip	= &finder->skip[0];
	unsigned int i;

	i = 0;
	do {
		skip[key[i]] = i_last - i;
		++i;
	} while (i < i_last);
}

static inline void
init_key_finders(void)
{
	key_finder_init_skip(&find_token);
	key_finder_init_skip(&find_email);
	key_finder_init_skip(&find_sender);
	key_finder_init_skip(&find_room);
}

static inline unsigned char *
key_finder_find(const struct KeyFinder *const restrict finder,
		const unsigned char *restrict text,
		const size_t length_text)
{
	unsigned int token;
	unsigned int i_match;

	const unsigned int length_key = finder->length;

	if (length_text < length_key)
		return NULL;

	const unsigned char *const restrict key = finder->key;
	const unsigned int *const restrict skip = &finder->skip[0];

	const unsigned char *const restrict text_until = text + length_text;

	const unsigned int i_last = length_key - 1;

	do {
		i_match = i_last;

		while (1) {
			token = (unsigned int) text[i_match];

			if (token != key[i_match])
				break;

			if (i_match == 0)
				return (unsigned char *) (text + length_key);

			--i_match;
		}

		text += skip[token];
	} while (text < text_until);

	return NULL;
}


static inline void
key_init(struct String *const restrict key,
	 unsigned char *restrict text)
{
	key->bytes = (char *) text;

	while (1) {
		switch (*text) {
		case '}':
		case ',':
			*text = '\0';

		case '\0':
			key->length = ((char *) text) - key->bytes;
			return;

		default:
			++text;
		}
	}
}


#define FIND_KEYS_FAILURE(REASON)				\
FAILURE_REASON("find_keys",					\
	       REASON)

#define FIND_KEY(KEY)						\
found = key_finder_find(&find_ ## KEY,				\
			text,					\
			length_text);				\
if (found == NULL) {						\
	*failure = FIND_KEYS_FAILURE("failed to find " #KEY);	\
	return false;						\
}								\
key_init(&(KEY),						\
	 found);						\
DEBUG("found " #KEY ": \"%s\"\n", (KEY).bytes)

static inline bool
find_keys(unsigned char *const restrict text,
	  const size_t length_text,
	  const char *restrict *const restrict failure)
{
	unsigned char *restrict found;

	FIND_KEY(token);

	if (!strings_equal(token.bytes,
			   TOKEN)) {
		*failure = FIND_KEYS_FAILURE("invalid token");
		return false;
	}

	FIND_KEY(email);
	FIND_KEY(sender);
	FIND_KEY(room);

	return true;
}

#if ANNOUNCE_CLIENT
#define CLIENT_MESSAGE_1	"\n\n----------\nreceived request from client { ip: "
#define CLIENT_MESSAGE_2	", port: "
#define CLIENT_MESSAGE_2_LENGTH	8
#define PUT_CLIENT_MESSAGE_2(PTR)					\
PUT_STRING_WIDTH(PTR, CLIENT_MESSAGE_2, CLIENT_MESSAGE_2_LENGTH)

#define CLIENT_MESSAGE_3	" }\n----------\n"
#define CLIENT_MESSAGE_3_LENGTH	14
#define PUT_CLIENT_MESSAGE_3(PTR)					\
PUT_STRING_WIDTH(PTR, CLIENT_MESSAGE_3, CLIENT_MESSAGE_3_LENGTH)

#define CLIENT_BUFFER_SIZE (  sizeof(CLIENT_MESSAGE_1)			\
			    + INET_ADDRSTRLEN				\
			    + CLIENT_MESSAGE_2_LENGTH			\
			    + 5 /* 65536 */				\
			    + CLIENT_MESSAGE_3_LENGTH)

static inline bool
announce_client(const struct sockaddr_in *const restrict client_address,
		const char *restrict *const restrict failure)
{
	char *restrict ptr;
	bool success;

	static char buffer[CLIENT_BUFFER_SIZE] = {
		CLIENT_MESSAGE_1
	};
	static char *const restrict ptr_start
	= &buffer[sizeof(CLIENT_MESSAGE_1)];


	success = inet_ntop_report(AF_INET,
				   &client_address->sin_addr,
				   ptr_start,
				   INET_ADDRSTRLEN,
				   failure);

	if (success) {
		ptr = ptr_start;
		do {
			++ptr;
		} while (*ptr != '\0');

		PUT_CLIENT_MESSAGE_2(ptr);

		ptr
		= put_uint(ptr,
			   (uintmax_t) HOST_SHORT(client_address->sin_port));

		PUT_CLIENT_MESSAGE_3(ptr);


		success = write_report(STDOUT_FILENO,
				       &buffer[0],
				       ptr - &buffer[0],
				       failure);
	}

	return success;
}
#endif /* if ANNOUNCE_CLIENT */


static inline void
exec_sendmail(const char *restrict *const restrict failure)
{
	extern char **environ;

	char *sendmail_argv[] = {
		"sendmail",
		email.bytes,
		NULL
	};

	DEBUG("exec_sendmail: execve \'%s\' w/ argv: {\"%s\", \"%s\", NULL}\n",
	      SENDMAIL_PATH,
	      sendmail_argv[0],
	      sendmail_argv[1]);
	execve_report(SENDMAIL_PATH,
		      sendmail_argv,
		      environ,
		      failure);
}

#define READ_REQUEST_FAILURE(REASON)				\
FAILURE_REASON("read_request",					\
	       REASON)

static inline bool
read_request(const int connect_descriptor,
	     unsigned char *restrict *const restrict request,
	     size_t *const restrict length_request,
	     const char *restrict *const restrict failure)
{
	static unsigned char request_buffer[REQUEST_BUFFER_SIZE];
	bool success;

	DEBUG("read_request (reading)\n");
	success = read_size_report(length_request,
				   connect_descriptor,
				   &request_buffer[0],
				   sizeof(request_buffer),
				   failure);

	if (success) {
		DEBUG("got request:\n\"\"\"\n%.*s\n\"\"\"\n",
		      (int) *length_request,
		      &request_buffer[0]);

		success = (*length_request < sizeof(request_buffer));

		if (success)
			*request = &request_buffer[0];
		else
			*failure = READ_REQUEST_FAILURE("buffer overflow");
	}

	return success;
}

#define EMAIL_BODY_1 "User "
#define PUT_EMAIL_BODY_1(PTR)						\
PUT_STRING_WIDTH(PTR,							\
		 EMAIL_BODY_1,						\
		 5)

#define EMAIL_BODY_2 " mentioned you in room "
#define PUT_EMAIL_BODY_2(PTR)						\
PUT_STRING_WIDTH(PTR,							\
		 EMAIL_BODY_2,						\
		 23)
#define EMAIL_BODY_3 "."
#define PUT_EMAIL_BODY_3(PTR)						\
PUT_CHAR(PTR, '.')

#define LENGTH_EMAIL_BODY_BASE						\
(sizeof(EMAIL_BODY_1 EMAIL_BODY_2 EMAIL_BODY_3) - 1)

#define WRITE_EMAIL_BODY_FAILURE(REASON)				\
FAILURE_REASON("write_email_body",					\
	       REASON)

static inline bool
write_email_body(const int sendmail_stdin,
		 const char *restrict *const restrict failure)
{
	char buffer[512];
	char *restrict ptr;

	if ((  LENGTH_EMAIL_BODY_BASE
	     + sender.length
	     + room.length) > sizeof(buffer)) {
		*failure = WRITE_EMAIL_BODY_FAILURE("buffer overflow");
		return false;
	}

	ptr = &buffer[0];

	PUT_EMAIL_BODY_1(ptr);

	ptr = put_string_size(ptr,
			      sender.bytes,
			      sender.length);

	PUT_EMAIL_BODY_2(ptr);

	ptr = put_string_size(ptr,
			      room.bytes,
			      room.length);

	PUT_EMAIL_BODY_3(ptr);

	DEBUG("write_email_body:\n\"\"\"\n%.*s\n\"\"\"\n",
	      (int) (ptr - &buffer[0]),
	      &buffer[0]);

	return write_report(sendmail_stdin,
			    &buffer[0],
			    ptr - &buffer[0],
			    failure);
}


static inline bool
handle_request(const int connect_descriptor,
	       const char *restrict *const restrict failure)
{
	int pipe_fds[2];
	pid_t process_id;
	bool success;
	unsigned char *restrict request;
	size_t length_request;

	success = read_request(connect_descriptor,
			       &request,
			       &length_request,
			       failure)
	       && close_report(connect_descriptor,
			       failure)
	       && find_keys(request,
			    length_request,
			    failure);

	if (success) {
		success = pipe_report(pipe_fds,
				      failure)
		       && fork_report(&process_id,
				      failure);

		if (success) {
			DEBUG("forked (handle_request)\n");

			if (process_id == 0) {
				/* child process */
				success = dup2_report(pipe_fds[0],
						      STDIN_FILENO,
						      failure)
				       && close_report(pipe_fds[1],
						       failure);

				if (success) {
					exec_sendmail(failure);
					success = false;
				}
			} else {
				/* parent process */
				success = close_report(pipe_fds[0],
						       failure)
				       && write_email_body(pipe_fds[1],
							   failure)
				       && close_report(pipe_fds[1],
						       failure);

				if (success) {
					exit(EXIT_SUCCESS);
					__builtin_unreachable();
				}
			}
		}
	}

	return success;
}

static inline bool
dispatch_request(const int connect_descriptor,
		 const int socket_descriptor,
		 const char *restrict *const restrict failure)
{
	bool success;

	pid_t process_id;

	success = fork_report(&process_id,
			      failure);

	if (success) {
		DEBUG("forked (dispatch_request)\n");

		if (process_id == 0) {
			/* child process */
			success = close_report(socket_descriptor,
					       failure)
			       && handle_request(connect_descriptor,
						 failure);
			/* should only return on failure */

		} else {
			/* parent process */
			success = close_report(connect_descriptor,
					       failure);
		}

	}

	return success;
}


int
main(void)
{
	int socket_descriptor;
	int connect_descriptor;
	const char *restrict failure;
	struct sockaddr_in client_address;
	socklen_t length_client_address;

	static struct sockaddr_in server_address;

	if (!socket_report(&socket_descriptor,
			   PF_INET,
			   SOCK_STREAM,
			   0,
			   &failure))
		exit_failure_print_message(failure);

	server_address.sin_family      = AF_INET;
	server_address.sin_addr.s_addr = NETWORK_LONG(INADDR_ANY);
	server_address.sin_port	       = NETWORK_SHORT(SERVER_PORT);

	if (!(   bind_report(socket_descriptor,
			     (struct sockaddr *) &server_address,
			     sizeof(server_address),
			     &failure)
	      && listen_report(socket_descriptor,
			       LISTEN_QUEUE,
			       &failure)))
		exit_failure_print_message(failure);


	init_key_finders();


	while (1) {
		length_client_address = sizeof(client_address);

		if (!(   accept_report(&connect_descriptor,
				       socket_descriptor,
				       (struct sockaddr *) &client_address,
				       &length_client_address,
				       &failure)
#if ANNOUNCE_CLIENT
		      && announce_client(&client_address,
					 &failure)
#endif /* if ANNOUNCE_CLIENT */
		      && dispatch_request(connect_descriptor,
					  socket_descriptor,
					  &failure)))
			exit_failure_print_message(failure);
	}

	/* unreachable */
        return 0;
}
