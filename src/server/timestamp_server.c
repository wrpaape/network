#include "network/network_utils.h"	/* network utils */
#include "time/time_string.h"		/* time|string_utils */

#define SERVER_PORT	13
#define LISTEN_QUEUE	128

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
				       (struct sockaddr *) NULL,
				       NULL,
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
