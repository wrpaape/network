#include "network/network_utils.h"	/* network utils */
#include "time/time_string.h"		/* time|string_utils */

#define SERVER_PORT	13
#define LISTEN_QUEUE	128

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

static inline bool
send_timestamp(const int connect_descriptor,
	       const char *restrict *const restrict failure)
{
	static struct Timestamp timestamp;
	static char buffer[SIZE_TIMESTAMP_STRING];
	bool success;

	success = timestamp_now_report(&timestamp,
				       failure);

	if (success) {
		timestamp_string_init(&buffer[0],
				      &timestamp);

		success = write_report(connect_descriptor,
				       &buffer[0],
				       sizeof(buffer),
				       failure);
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
		if (!(   accept_report(&connect_descriptor,
				       socket_descriptor,
				       (struct sockaddr *) &client_address,
				       &length_client_address,
				       &failure)
		      && announce_client(&client_address,
					 &failure)
		      && send_timestamp(connect_descriptor,
					&failure)
		      && close_report(connect_descriptor,
				      &failure)))
			exit_failure_print_message(failure);
	}

	/* unreachable */
	return 0;
}
