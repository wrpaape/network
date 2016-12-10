#include "network/network_utils.h"	/* network utils */
#include "time/time_string.h"		/* put http_date */

#ifndef SENDMAIL_PATH
#define SENDMAIL_PATH "/usr/sbin/sendmail"
#endif /* ifdef SENDMAIL_PATH */

#define DO_DEBUG		1
#define REQUEST_BUFFER_SIZE	4096
#define SERVER_PORT		80
#define LISTEN_QUEUE		128
#define TOKEN			"dummy_token"


#if DO_DEBUG
#	define DEBUG(...)						\
printf("(proccess: %d)\t",						\
       (int) getpid());							\
printf(__VA_ARGS__);							\
fflush(stdout)
#else
#	define DEBUG(...)
#endif /* if DO_DEBUG */


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct KeyFinder {
	const unsigned char *key;
	unsigned int length;
	unsigned int skip[UCHAR_MAX + 1];
};

#define KEY_FIELD(KEY)  ("\"" KEY "\":")
#define LENGTH_KEY(KEY) (sizeof(KEY_FIELD(KEY)) - 1)
#define KEY_FINDER_INIT(KEY) {						\
	.key    = (const unsigned char *) KEY_FIELD(KEY),		\
	.length = LENGTH_KEY(KEY),					\
	.skip   = {							\
		[0 ... UCHAR_MAX] = LENGTH_KEY(KEY)			\
	}								\
}

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static struct KeyFinder find_json = {
	.key    = (const unsigned char *) "\r\n\r\n{",
	.length = 5,
	.skip   = {
		[0 ... UCHAR_MAX]  = 5
	}
};
static struct String json;

static struct KeyFinder find_token     = KEY_FINDER_INIT("token");

static struct KeyFinder find_email     = KEY_FINDER_INIT("email");
static struct String email;
static struct KeyFinder find_name      = KEY_FINDER_INIT("name");
static struct String name;
/* static struct KeyFinder find_mention   = KEY_FINDER_INIT("mention"); */
/* static struct String mention; */
static struct KeyFinder find_from      = KEY_FINDER_INIT("from");
static struct String from;
static struct KeyFinder find_room      = KEY_FINDER_INIT("room");
static struct String room;
static struct KeyFinder find_message   = KEY_FINDER_INIT("message");
static struct String message;
/* static struct KeyFinder find_mentions  = KEY_FINDER_INIT("mentions"); */
/* static struct String mentions; */
static struct KeyFinder find_timestamp = KEY_FINDER_INIT("timestamp");
time_t timestamp_time;

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
	key_finder_init_skip(&find_json);
	key_finder_init_skip(&find_token);
	key_finder_init_skip(&find_email);
	key_finder_init_skip(&find_name);
	/* key_finder_init_skip(&find_mention); */
	key_finder_init_skip(&find_from);
	key_finder_init_skip(&find_room);
	key_finder_init_skip(&find_message);
	/* key_finder_init_skip(&find_mentions); */
	key_finder_init_skip(&find_timestamp);
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

#define KEY_INIT_STRING_FAILURE(REASON)					\
FAILURE_REASON("key_init_string",					\
	       REASON)
#define KEY_INIT_STRING_FAILURE_BAD_KEY					\
KEY_INIT_STRING_FAILURE("ill-formed key")
#define KEY_INIT_STRING_FAILURE_BAD_JSON				\
KEY_INIT_STRING_FAILURE("ill-formed JSON")
static inline bool
key_init_string(struct String *const restrict key,
		unsigned char *restrict text,
		const char *restrict *const restrict failure)
{
	if (*text != '\"') {
		*failure = KEY_INIT_STRING_FAILURE_BAD_KEY;
		return false;
	}

	++text;
	key->bytes = (char *) text;

	while (1) {
		switch (*text) {
		case '\"':
			*text = '\0';
			key->length = ((char *) text) - key->bytes;
			return true;

		case '\0':
			*failure = KEY_INIT_STRING_FAILURE_BAD_JSON;
			return false;

		case '\\':
			++text;
			if (*text == '\0') {
				*failure = KEY_INIT_STRING_FAILURE_BAD_KEY;
				return false;
			}
			/* fall through */
		default:
			++text;
		}
	}
}

#define PARSE_TIMESTAMP_FAILURE(REASON)					\
FAILURE_REASON("parse_timestamp",					\
	       REASON)
#define PARSE_TIMESTAMP_FAILURE_BAD_INT					\
PARSE_TIMESTAMP_FAILURE("improper integer")
#define PARSE_TIMESTAMP_FAILURE_BAD_JSON				\
PARSE_TIMESTAMP_FAILURE("ill-formed JSON")

static inline bool
parse_timestamp(unsigned char *restrict text,
		const char *restrict *const restrict failure)
{
	uintmax_t timestamp_parsed;

	char *const restrict timestamp_string = (char *) text;

	while (1) {
		switch (*text) {
		case '}':
		case ',':
			*text = '\0';

			const bool success
			= (parse_uint(&timestamp_parsed,
				      timestamp_string) != NULL);

			if (success)
				timestamp_time = (time_t) timestamp_parsed;
			else
				*failure = PARSE_TIMESTAMP_FAILURE_BAD_INT;

			return success;

		case '\0':
			*failure = PARSE_TIMESTAMP_FAILURE_BAD_JSON;
			return false;

		default:
			++text;
		}
	}
}

static inline bool
find_http_json(unsigned char *const restrict request,
	       const size_t length_request,
	       const char *restrict *const restrict failure)
{
	json.bytes = (char *) key_finder_find(&find_json,
					      request,
					      length_request);

	const bool success = (json.bytes != NULL);

	if (success)
		json.length = ((char *) (request + length_request))
			    - json.bytes;
	else
		*failure = FAILURE_REASON("find_http_json",
					  "failed to find HTTP POST json");

	return success;
}


static inline bool
find_keys(const char *restrict *const restrict failure)
{
	unsigned char *restrict found;

#define FIND_KEY(KEY)							\
found = key_finder_find(&find_ ## KEY,					\
			(const unsigned char *) json.bytes,		\
			json.length);					\
if (found == NULL) {							\
	*failure = FAILURE_REASON("find_keys", "failed to find " #KEY);	\
	return false;							\
}									\

#define FIND_KEY_STRING(KEY)						\
FIND_KEY(KEY);								\
if (!key_init_string(&(KEY),						\
		     found,						\
		     failure))						\
	return false


	FIND_KEY_STRING(email);
	FIND_KEY_STRING(name);
	FIND_KEY_STRING(from);
	FIND_KEY_STRING(room);
	FIND_KEY_STRING(message);
	FIND_KEY(timestamp);
	return parse_timestamp(found,
			       failure);
}




static inline bool
verify_request(void)
{
	char *restrict ptr;
	char *restrict token;

	token = (char *) key_finder_find(&find_token,
					 (const unsigned char *) json.bytes,
					 json.length);

	if (token == NULL) {
		DEBUG("failed to find token\n");
		return false;
	}


	if (*token != '\"') {
		DEBUG("ill-formed token\n");
		return false;
	}

	++token;
	ptr = token;

	while (1) {
		switch (*ptr) {
		case '\"':
			*ptr = '\0';
			DEBUG("found token: \"%s\"\n",
			      token);

			return strings_equal(token,
					     TOKEN);

		case '\0':
			DEBUG("ill-formed JSON\n");
			return false;

		case '\\':
			++ptr;
			if (*ptr == '\0') {
				DEBUG("ill-formed token\n");
				return false;
			}
			/* fall through */
		default:
			++ptr;
		}
	}
}

#if DO_DEBUG
#define CLIENT_MESSAGE_1	"\n\n\n----------\nreceived request from client { ip: "
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
#endif /* if DO_DEBUG */


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



static inline bool
send_response(const int connect_descriptor,
	      const char *const restrict http_status,
	      const char *restrict *const restrict failure)
{
	char buffer[256];
	char *restrict ptr;
	struct Timestamp timestamp;

	if (!timestamp_now_report(&timestamp,
				  failure))
		return false;

	ptr = &buffer[0];

	PUT_STRING_WIDTH(ptr, "HTTP/1.1 ", 9);

	ptr = put_string(ptr,
			 http_status);


	PUT_STRING_WIDTH(ptr, "\r\nDate: ", 8);

	ptr = put_http_date(ptr,
			    &timestamp);

	PUT_STRING_WIDTH(ptr,
			 "\r\nConnection: close\r\n\r\n",
			 23);

	DEBUG("send_response:\n\"\"\"\n%.*s\n\"\"\"\n",
	      (int) (ptr - &buffer[0]),
	      &buffer[0]);

	return write_report(connect_descriptor,
			    &buffer[0],
			    ptr - &buffer[0],
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

// From: ejabberd <donotreply@ejabberd.com>
// To: [NAME] <[EMAIL]>
// Subject: [FROM] mentioned you in room "[ROOM]"
// MIME-Version: 1.0
// Content-Type: text/html
//
// <html>
// <head>
// <title>ejabberd Mention Notification</title>
// </head>
// <body>
// <strong>[FROM]</strong> mentioned you in room <strong>[ROOM]</strong> on [TIME]:
// <blockquote>[MESSAGE]</blockquote>
// </body>
// </html>

#define EMAIL_1								\
"From: ejabberd <donotreply@ejabberd.com>\n"				\
"To: "
#define PUT_EMAIL_1(PTR) PTR = put_string_size(PTR, EMAIL_1, sizeof(EMAIL_1) - 1)

#define EMAIL_2 " <"
#define PUT_EMAIL_2(PTR) PUT_STRING_WIDTH(PTR, EMAIL_2, 2)

#define EMAIL_3 ">\nSubject: "
#define PUT_EMAIL_3(PTR) PUT_STRING_WIDTH(PTR, EMAIL_3, 11)

#define EMAIL_4 " mentioned you in room \""
#define PUT_EMAIL_4(PTR) PUT_STRING_WIDTH(PTR, EMAIL_4, 24)

#define EMAIL_5								\
"\"\n"									\
"MIME-Version: 1.0\n"							\
"Content-Type: text/html\n"						\
"\n"									\
"<html>\n"								\
"<head>\n"								\
"<title>ejabberd Mention Notification</title>\n"			\
"</head>\n"								\
"<body>\n"								\
"<strong>"
#define PUT_EMAIL_5(PTR) PTR = put_string_size(PTR, EMAIL_5, sizeof(EMAIL_5) - 1)

#define EMAIL_6 "</strong> mentioned you in room <strong>"
#define PUT_EMAIL_6(PTR) PTR = put_string_size(PTR, EMAIL_6, sizeof(EMAIL_6) - 1)

#define EMAIL_7 "</strong> on "
#define PUT_EMAIL_7(PTR) PUT_STRING_WIDTH(PTR, EMAIL_7, 13)

#define EMAIL_8								\
".\n"									\
"<blockquote>"
#define PUT_EMAIL_8(PTR) PUT_STRING_WIDTH(PTR, EMAIL_8, 14)


#define EMAIL_9								\
"</blockquote>\n"							\
"</body>\n"								\
"</html>"
#define PUT_EMAIL_9(PTR) PUT_STRING_WIDTH(PTR, EMAIL_9, 29)


static inline char *
put_time_element(char *restrict string)
{
	struct Timestamp timestamp;

	timestamp_init(&timestamp,
		       timestamp_time);

	return put_http_time_element(string,
				     &timestamp);
}

static inline bool
write_email_body(const int sendmail_stdin,
		 const char *restrict *const restrict failure)
{
	char buffer[REQUEST_BUFFER_SIZE];
	char *restrict ptr;

	ptr = &buffer[0];

	PUT_EMAIL_1(ptr);

	ptr = put_string_size(ptr,
			      name.bytes,
			      name.length);

	PUT_EMAIL_2(ptr);

	ptr = put_string_size(ptr,
			      email.bytes,
			      email.length);

	PUT_EMAIL_3(ptr);

	ptr = put_string_size(ptr,
			      from.bytes,
			      from.length);

	PUT_EMAIL_4(ptr);

	ptr = put_string_size(ptr,
			      room.bytes,
			      room.length);

	PUT_EMAIL_5(ptr);

	ptr = put_string_size(ptr,
			      from.bytes,
			      from.length);

	PUT_EMAIL_6(ptr);

	ptr = put_string_size(ptr,
			      room.bytes,
			      room.length);

	PUT_EMAIL_7(ptr);

	ptr = put_time_element(ptr);

	PUT_EMAIL_8(ptr);

	ptr = put_string_size(ptr,
			      message.bytes,
			      message.length);

	PUT_EMAIL_9(ptr);

	DEBUG("write_email_body:\n\"\"\"\n%.*s\n\"\"\"\n",
	      (int) (ptr - &buffer[0]),
	      &buffer[0]);

	return write_report(sendmail_stdin,
			    &buffer[0],
			    ptr - &buffer[0],
			    failure);
}


static inline bool
send_email(const char *restrict *const restrict failure)
{
	int pipe_fds[2];
	pid_t process_id;
	bool success;

	success = pipe_report(pipe_fds,
			      failure);

	if (success) {
		success = fork_report(&process_id,
				      failure);

		if (success) {
			DEBUG("forked (send_email)\n");

			if (process_id == 0) {
				/* child process */
				success = dup2_report(pipe_fds[0],
						      STDIN_FILENO,
						      failure);
				if (success) {
					success = close_report(pipe_fds[1],
							       failure);

					if (success) {
						exec_sendmail(failure);
						success = false;

					} else {
						close_muffle(pipe_fds[0]);
					}

				} else {
					close_muffle(pipe_fds[0]);
					close_muffle(pipe_fds[1]);
				}
			} else {
				/* parent process */
				success = close_report(pipe_fds[0],
						       failure);

				if (success) {
				       success = write_email_body(pipe_fds[1],
								  failure);

				       if (success) {
					       success
					       = close_report(pipe_fds[1],
							      failure);

						if (success) {
							exit(EXIT_SUCCESS);
							__builtin_unreachable();
						}
				       } else {
					       close_muffle(pipe_fds[1]);

				       }
				} else {
					close_muffle(pipe_fds[1]);
				}
			}
		} else {
			close_muffle(pipe_fds[0]);
			close_muffle(pipe_fds[1]);
		}
	}

	return success;
}


static inline bool
handle_request(const int connect_descriptor,
	       const char *restrict *const restrict failure)
{
	bool success;
	unsigned char *restrict request;
	size_t length_request;

	success = read_request(connect_descriptor,
			       &request,
			       &length_request,
			       failure)
	       && find_http_json(request,
				 length_request,
				 failure);

	if (success) {
		if (verify_request()) {
			DEBUG("valid token\n");
			success = send_response(connect_descriptor,
						"200 OK",
						failure);

			if (success) {
				success = close_report(connect_descriptor,
						       failure)
				       && find_keys(failure)
				       && send_email(failure);
				/* should only return on failure */

			} else {
				close_muffle(connect_descriptor);
			}

		} else {
			DEBUG("token not found or invalid\n");
			success = send_response(connect_descriptor,
						"403 Forbidden",
						failure);

			if (success) {
				success = close_report(connect_descriptor,
						       failure);
				if (success) {
					exit(EXIT_SUCCESS);
					__builtin_unreachable();
				}
			} else {
				close_muffle(connect_descriptor);
			}
		}
	} else {
		close_muffle(connect_descriptor);
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
#if DO_DEBUG
		      && announce_client(&client_address,
					 &failure)
#endif /* if DO_DEBUG */
		      && dispatch_request(connect_descriptor,
					  socket_descriptor,
					  &failure)))
			exit_failure_print_message(failure);
	}

	/* unreachable */
        return 0;
}
