#include "network/network_utils.h"	/* network utils */

#ifndef SENDMAIL_PATH
#define SENDMAIL_PATH "usr/sbin/sendmail"
#endif /* ifdef SENDMAIL_PATH */

#ifdef ANNOUNCE_CLIENT
#define CLIENT_MESSAGE_1	"received request from client { ip: "

#define CLIENT_MESSAGE_2	", port: "
#define CLIENT_MESSAGE_2_LENGTH	8
#define PUT_CLIENT_MESSAGE_2(PTR)					\
PUT_STRING_WIDTH(PTR, CLIENT_MESSAGE_2, CLIENT_MESSAGE_2_LENGTH)

#define CLIENT_MESSAGE_3	" }\n"
#define CLIENT_MESSAGE_3_LENGTH	3
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
			   (uintmax_t) NETWORK_SHORT(client_address->sin_port));

		PUT_CLIENT_MESSAGE_3(ptr);


		success = write_report(STDOUT_FILENO,
				       &buffer[0],
				       ptr - &buffer[0],
				       failure);
	}

	return success;
}
#endif /* ifdef ANNOUNCE_CLIENT */


static inline void
spawn_sendmail(const char *restrict email_address)
__attribute__((noreturn));

static inline void
spawn_sendmail(char *restrict email_address)
{
	extern char **environ;

	const char *restrict failure;

	char *sendmail_argv[] = {
		"sendmail",
		email_address,
		NULL
	};

	execve_report(SENDMAIL_PATH,
		      sendmail_argv,
		      environ,
		      &failure);

	exit_failure_print_message(failure);
}



int
main(void)
{
	int socket_descriptor;
	int connect_descriptor;
	const char *restrict failure;
	struct sockaddr_in client_address;
	socklen_t length_client_address;

	if (!socket_report(&socket_descriptor,
			   PF_INET,
			   SOCK_STREAM,
			   0,
			   &failure))
		exit_failure_print_message(failure);

	server_address.sin_family	= AF_INET;
	server_address.sin_addr.s_addr	= NETWORK_LONG(INADDR_ANY);
	server_address.sin_port		= NETWORK_SHORT(SERVER_PORT);

	if (!bind_report(socket_descriptor,
			 (struct sockaddr *) &server_address,
			 sizeof(server_address),
			 &failure))
		exit_failure_print_message(failure);

	if (!listen_report(socket_descriptor,
			   LISTEN_QUEUE,
			   &failure))
		exit_failure_print_message(failure);


	while (1) {
		length_client_address = sizeof(client_address);

		if (accept_report(&connect_descriptor,
				  socket_descriptor,
				  (struct sockaddr *) &client_address,
				  &length_client_address,
				  &failure)) {

		} else {
			exit_failure_print_message(failure);
		}
	}

	/* unreachable */
        return 0;
}
