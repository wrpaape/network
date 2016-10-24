#include "network/network_utils.h"	/* network utils */
#include "time/time_string.h"		/* time|string_utils */

#define SERVER_IP	"127.0.0.1"
#define SERVER_PORT	13

#define INET_PTON_FAILURE FAILURE_REASON("inet_pton",			\
					 "invalid IP \'" SERVER_IP "\'")

int
main(void)
{
	int socket_descriptor;
	static char timestamp[SIZE_TIMESTAMP_STRING + 1];	/* \n */
	static char *restrict buffer = &timestamp[0];
	size_t rem_size;
	static struct sockaddr_in server_address;
	const char *restrict failure;
	size_t size_read;


	if (!socket_report(&socket_descriptor,
			   PF_INET,
			   SOCK_STREAM,
			   0,
			   &failure))
		exit_failure_print_message(failure);

	server_address.sin_family = AF_INET;
	server_address.sin_port	  = NETWORK_SHORT(SERVER_PORT);

	if (!inet_pton(AF_INET,
		       SERVER_IP,
		       &server_address.sin_addr))
		exit_failure_dump_buffer(INET_PTON_FAILURE,
					 sizeof(INET_PTON_FAILURE));

	if (!connect_report(socket_descriptor,
			    (struct sockaddr *) &server_address,
			    sizeof(server_address),
			    &failure))
		exit_failure_print_message(failure);

	rem_size = SIZE_TIMESTAMP_STRING;

	while (1) {
		if (!read_size_report(&size_read,
				      socket_descriptor,
				      buffer,
				      rem_size,
				      &failure))
			exit_failure_print_message(failure);

		if (size_read == 0)
			break;

		buffer	 += size_read;
		rem_size -= size_read;
	}

	*buffer = '\n';

	if (!(   close_report(socket_descriptor,
			      &failure)
	      && write_report(STDOUT_FILENO,
			      &timestamp[0],
			      sizeof(timestamp),
			      &failure)))
	    exit_failure_print_message(failure);

	return 0;
}
