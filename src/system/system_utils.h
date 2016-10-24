#ifndef MYSQL_SEED_SYSTEM_SYSTEM_UTILS_H_
#define MYSQL_SEED_SYSTEM_SYSTEM_UTILS_H_


/* EXTERNAL DEPENDENCIES
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

#include <sys/types.h>	/* ssize_t, chmod API, LINUX compatibility */

#ifdef WIN32
#	include <windows.h>	/* DeviceIoControl */
#	include <winsock2.h>	/* socket, bind, listen, connect, accept */
#	include <iphlpapi.h>	/* GetAdaptersAddress */
#	include <ws2tcpip.h>	/* getaddrinfo */
#else
#	include <sys/ioctl.h>	/* iotcl */
#	include <sys/socket.h>	/* socket, bind, listen, connect, accept */
#	include <net/if.h>	/* ifreq, ifconf */
#	include <netdb.h>	/* getaddrinfo */
#endif /* ifdef WIN32 */

#ifdef OSX
#	include <sys/sysctl.h>	/* sysctl */
#endif /* ifdef OSX */

#include <signal.h>		/* sig_t, signal, raise */
#include <sys/uio.h>		/* read, write */
#include <sys/stat.h>		/* mkdir */
#include <sys/param.h>		/* MAXPATHLEN */
#include "utils/fail_switch.h"	/* stdbool, errno, FAIL_SWITCH */
#include "utils/closure.h"	/* HandlerClosure */

/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * EXTERNAL DEPENDENCIES
 *
 *
 * TYPEDEFS, ENUM AND STRUCT DEFINITIONS
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

/* included from <sys/ioctl.h>
 *
 * struct winsize {
 *	unsigned short ws_row;		rows, in characters
 *	unsigned short ws_col;		columns, in characters
 *	unsigned short ws_xpixel;	horizontal size, pixels
 *	unsigned short ws_ypixel;	vertical size, pixels
 * }; */

/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * TYPEDEFS, ENUM AND STRUCT DEFINITIONS
 *
 *
 * CONSTANTS
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */
/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * CONSTANTS
 *
 *
 * FUNCTION-LIKE MACROS
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

#ifdef WIN32
#	define get_adapters_addresses_imp(FAMILY,			\
					  FLAGS,			\
					  RESERVED,			\
					  ADAPTER_ADDRESSES,		\
					  SIZE_POINTER)			\
	GetAdaptersAddresses(FAMILY,					\
			     FLAGS,					\
			     RESERVED,					\
			     ADAPTER_ADDRESSES,				\
			     SIZE_POINTER)
#	define size_adapters_addresses_imp(FAMILY,			\
					   FLAGS,			\
					   RESERVED,			\
					   SIZE_POINTER)		\
	GetAdaptersAddresses(FAMILY,					\
			     FLAGS,					\
			     RESERVED,					\
			     NULL,					\
			     SIZE_POINTER)
#else
/* ioctl wrappers */
#	define interface_name_to_index_imp(NAME)			\
	if_nametoindex(NAME)

#	define get_interface_networks_imp(CONFIG, DEVICE_DESCRIPTOR)	\
	ioctl(DEVICE_DESCRIPTOR, SIOCGIFCONF, CONFIG)

#	define get_device_active_flags_imp(REQUEST, DEVICE_DESCRIPTOR)	\
	ioctl(DEVICE_DESCRIPTOR, SIOCGIFFLAGS, REQUEST)

#	define get_winsize_imp(WINDOW, FILE_DESCRIPTOR)			\
	ioctl(FILE_DESCRIPTOR, TIOCGWINSZ, WINDOW)
#endif /* ifdef WIN32 */

#ifdef LINUX
#	define get_interface_index_imp(REQUEST, DEVICE_DESCRIPTOR)	\
	ioctl(DEVICE_DESCRIPTOR, SIOCGIFINDEX, REQUEST)

#	define get_hardware_address_imp(REQUEST, DEVICE_DESCRIPTOR)	\
	ioctl(DEVICE_DESCRIPTOR, SIOCGIFHWADDR, REQUEST)
#endif /* ifdef LINUX */


/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * FUNCTION-LIKE MACROS
 *
 *
 * HELPER FUNCTIONS
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * HELPER FUNCTIONS
 *
 *
 * TOP-LEVEL FUNCTIONS
 * ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ */

#define FAIL_SWITCH_ERRNO_FAILURE	-1
#define FAIL_SWITCH_FAILURE_POINTER	failure

/* socket */
inline bool
socket_status(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol)
{
	*socket_descriptor = socket(domain,
				    type,
				    protocol);

	return *socket_descriptor >= 0;
}

inline void
socket_muffle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol)
{
	*socket_descriptor = socket(domain,
				    type,
				    protocol);
}

#define FAIL_SWITCH_ROUTINE socket
inline bool
socket_report(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      const char *restrict *const restrict failure)
{

	*socket_descriptor = socket(domain,
				    type,
				    protocol);

	if (LIKELY(*socket_descriptor >= 0))
		return true;

	switch (errno) {
	FAIL_SWITCH_ERRNO_CASE_1(EACCES,
				 "Permission to create a socket of the "
				 "specified type and/or protocol is denied.")
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "The specified address family is not supported"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(EMFILE,
				 "The per-process descriptor table is full.")
	FAIL_SWITCH_ERRNO_CASE_1(ENFILE,
				 "The system file table is full.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOBUFS,
				 "Insufficient buffer space is available. The "
				 "socket cannot be created until sufficient "
				 "resources are freed.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOMEM,
				 "Insufficient memory was available to fulfill "
				 "the request.")

	FAIL_SWITCH_ERRNO_CASE_1(EPROTONOSUPPORT,
				 "The protocol type or the specified protocol "
				 "is not supported within this domain.")
	FAIL_SWITCH_ERRNO_CASE_1(EPROTOTYPE,
				 "The socket type is not supported by the "
				 "protocol.")
	FAIL_SWITCH_ERRNO_DEFAULT_CASE()
	}
}

inline void
socket_handle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      Handler *const handle,
	      void *arg)
{
	const char *restrict failure;

	if (LIKELY(socket_report(socket_descriptor,
				 domain,
				 type,
				 protocol,
				 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
socket_handle_cl(int *const restrict socket_descriptor,
		 const int domain,
		 const int type,
		 const int protocol,
		 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(socket_report(socket_descriptor,
				 domain,
				 type,
				 protocol,
				 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}

/* bind */
inline bool
bind_status(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address)
{
	return bind(socket_descriptor,
		    address,
		    length_address) == 0;
}

inline void
bind_muffle(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address)
{
	(void) bind(socket_descriptor,
		    address,
		    length_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define	FAIL_SWITCH_ROUTINE bind
inline bool
bind_report(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address,
	    const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(socket_descriptor,
			       address,
			       length_address)
	FAIL_SWITCH_ERRNO_CASE_2(EACCES,
				 "The requested address is protected, and the "
				 "current user has inadequate permission to "
				 "access it.",
				 "A component of the path prefix does not allow"
				 " searching or the node's parent directory "
				 "denies write permission.")
	FAIL_SWITCH_ERRNO_CASE_1(EADDRINUSE,
				 "The specified address is already in use.")
	FAIL_SWITCH_ERRNO_CASE_1(EADDRNOTAVAIL,
				 "The specified address is not available from "
				 "the local machine.")
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "'address' is not valid for the address family"
				 " of 'socket_descriptor'.")
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'socket_descriptor' is not a valid file "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EDESTADDRREQ,
				 "'socket_descriptor' is a null pointer.")
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The 'address' parameter is not in a valid "
				 "part of the user address space.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'socket_descriptor' is already bound to an "
				 "address and the protocol does not support "
				 "binding to a new address.  Alternatively, "
				 "socket may have been shut down.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "'socket_descriptor' does not refer to a "
				 "socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "'socket_descriptor' is not of a type that can"
				 " be bound to an address.")
	FAIL_SWITCH_ERRNO_CASE_1(EEXIST,
				 "A file already exists at the pathname. "
				 "unlink(2) it first.")
	FAIL_SWITCH_ERRNO_CASE_1(EIO,
				 "An I/O error occurred while making the "
				 "directory entry or allocating the inode.")
	FAIL_SWITCH_ERRNO_CASE_1(EISDIR,
				 "An empty pathname was specified.")
	FAIL_SWITCH_ERRNO_CASE_1(ELOOP,
				 "Too many symbolic links were encountered in "
				 "translating the pathname.  This is taken to "
				 "be indicative of a looping symbolic link.")
	FAIL_SWITCH_ERRNO_CASE_1(ENAMETOOLONG,
				 "A component of a pathname exceeded {NAME_MAX}"
				 " characters, or an entire path name exceeded "
				 "{PATH_MAX} characters.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOENT,
				 "A component of the path name does not refer "
				 "to an existing file.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTDIR,
				 "A component of the path prefix is not a "
				 "directory.")
	FAIL_SWITCH_ERRNO_CASE_1(EROFS,
				 "The name would reside on a read-only file "
				 "system.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
bind_handle(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address,
	    Handler *const handle,
	    void *arg)
{
	const char *restrict failure;

	if (LIKELY(bind_report(socket_descriptor,
			       address,
			       length_address,
			       &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
bind_handle_cl(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(bind_report(socket_descriptor,
			       address,
			       length_address,
			       &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* listen */
inline bool
listen_status(const int socket_descriptor,
	      const int backlog)
{
	return listen(socket_descriptor,
		      backlog) == 0;
}

inline void
listen_muffle(const int socket_descriptor,
	      const int backlog)
{
	(void) listen(socket_descriptor,
		      backlog);
}

#undef	FAIL_SWITCH_ROUTINE
#define	FAIL_SWITCH_ROUTINE listen
inline bool
listen_report(const int socket_descriptor,
	      const int backlog,
	      const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(socket_descriptor,
			       backlog)
	FAIL_SWITCH_ERRNO_CASE_1(EACCES,
				 "The current process has insufficient "
				 "privileges.")
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "The argument 'socket_descriptor' is not a "
				 "valid file descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EDESTADDRREQ,
				 "The socket is not bound to a local address "
				 "and the protocol does not support listening "
				 "on an unbound socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'socket_descriptor' is already connected.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "The argument 'socket_descriptor' does not "
				 "reference a socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "The socket is not of a type that supports the"
				 " operation listen().")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
listen_handle(const int socket_descriptor,
	      const int backlog,
	      Handler *const handle,
	      void *arg)
{
	const char *restrict failure;

	if (LIKELY(listen_report(socket_descriptor,
				 backlog,
				 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
listen_handle_cl(const int socket_descriptor,
		 const int backlog,
		 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(listen_report(socket_descriptor,
				 backlog,
				 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* connect */
inline bool
connect_status(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address)
{
	return connect(socket_descriptor,
		       address,
		       length_address) == 0;
}

inline void
connect_muffle(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address)
{
	(void) connect(socket_descriptor,
		       address,
		       length_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define	FAIL_SWITCH_ROUTINE connect
inline bool
connect_report(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(socket_descriptor,
			       address,
			       length_address)
	FAIL_SWITCH_ERRNO_CASE_3(EACCES,
				 "The destination address is a broadcast "
				 "address and the socket option '"
				 "SO_BROADCAST' is not set.",
				 "Search permission is denied for a component "
				 "of the path prefix.",
				 "Write access to the named socket is denied.")
	FAIL_SWITCH_ERRNO_CASE_1(EADDRINUSE,
				 "The address is already in use.")
	FAIL_SWITCH_ERRNO_CASE_1(EADDRNOTAVAIL,
				 "The specified address is not available on "
				 "this machine.")
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "Addresses in the specified address family "
				 "cannot be used with this socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EALREADY,
				 "The socket is non-blocking and a previous "
				 "connection attempt has not yet been completed"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'socket_descriptor' is not a valid descriptor"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(ECONNREFUSED,
				 "The attempt to connect was ignored (because "
				 "the target is not listening for connections) "
				 "or explicitly rejected.")
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The 'address' parameter specifies an area "
				 "outside the process address space.")
	FAIL_SWITCH_ERRNO_CASE_1(EHOSTUNREACH,
				 "The target host cannot be reached (e.g., down"
				 ", disconnected).")
	FAIL_SWITCH_ERRNO_CASE_1(EINPROGRESS,
				 "The socket is non-blocking and the connection"
				 " cannot be completed immediately. It is "
				 "possible to select(2) for completion by "
				 "selecting the socket for writing.")
	FAIL_SWITCH_ERRNO_CASE_1(EINTR,
				 "Its execution was interrupted by a signal.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "An invalid argument was detected (e.g., '"
				 "length_address' is not valid for the address "
				 "family, the specified address family is "
				 "invalid).")
	FAIL_SWITCH_ERRNO_CASE_1(EISCONN,
				 "The socket is already connected.")
	FAIL_SWITCH_ERRNO_CASE_1(ENETDOWN,
				 "The local network interface is not "
				 "functioning.")
	FAIL_SWITCH_ERRNO_CASE_1(ENETUNREACH,
				 "The network isn't reachable from this host.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOBUFS,
				 "The system call was unable to allocate a "
				 "needed memory buffer.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "'socket_descriptor' is not a file descriptor "
				 "for a socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "Because 'socket_descriptor' is listening, no "
				 "connection is allowed.")
	FAIL_SWITCH_ERRNO_CASE_1(EPROTOTYPE,
				 "'address' has a different type than the "
				 "socket that is bound to the specified peer "
				 "address.")
	FAIL_SWITCH_ERRNO_CASE_1(ETIMEDOUT,
				 "Connection establishment timed out without "
				 "establishing a connection.")
	FAIL_SWITCH_ERRNO_CASE_1(ECONNRESET,
				 "Remote host reset the connection request.")
	FAIL_SWITCH_ERRNO_CASE_1(EIO,
				 "An I/O error occurred while reading from or "
				 "writing to the file system.")
	FAIL_SWITCH_ERRNO_CASE_1(ELOOP,
				 "Too many symbolic links were encountered in "
				 "translating the pathname. This is taken to be"
				 " indicative of a looping symbolic link.")
	FAIL_SWITCH_ERRNO_CASE_1(ENAMETOOLONG,
				 "A component of a pathname exceeded {NAME_MAX}"
				 " characters, or an entire path name exceeded "
				 "{PATH_MAX} characters.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOENT,
				 "The named socket does not exist.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTDIR,
				 "A component of the path prefix is not a directory.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
connect_handle(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       Handler *const handle,
	       void *arg)
{
	const char *restrict failure;

	if (LIKELY(connect_report(socket_descriptor,
				  address,
				  length_address,
				  &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
connect_handle_cl(const int socket_descriptor,
		  const struct sockaddr *const restrict address,
		  const socklen_t length_address,
		  const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(connect_report(socket_descriptor,
				  address,
				  length_address,
				  &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* accept */
inline bool
accept_status(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address)
{
	*connect_descriptor = accept(socket_descriptor,
				     address,
				     length_address);

	return *connect_descriptor >= 0;
}

inline void
accept_muffle(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address)
{
	*connect_descriptor = accept(socket_descriptor,
				     address,
				     length_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE accept
inline bool
accept_report(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address,
	      const char *restrict *const restrict failure)
{
	*connect_descriptor = accept(socket_descriptor,
				     address,
				     length_address);

	if (LIKELY(*connect_descriptor >= 0))
		return true;

	switch (errno) {
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'socket_descriptor' is not a valid file "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(ECONNABORTED,
				 "The connection to 'socket_descriptor' has "
				 "been aborted.")
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The address parameter is not in a writable "
				 "part of the user address space.")
	FAIL_SWITCH_ERRNO_CASE_1(EINTR,
				 "The accept() system call was terminated by a "
				 "signal.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'socket_descriptor' is unwilling to accept "
				 "connections.")
	FAIL_SWITCH_ERRNO_CASE_2(EMFILE,
				 "The per-process descriptor table is full.",
				 "The system file table is full.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOMEM,
				 "Insufficient memory was available to complete"
				 " the operation.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "'socket_descriptor' references a file type "
				 "other than a socket.")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "'socket_descriptor' is not of type '"
				 "SOCK_STREAM' and thus does not accept "
				 "connections.")
	FAIL_SWITCH_ERRNO_CASE_1(EWOULDBLOCK,
				 "'socket_descriptor' is marked as non-blocking"
				 "and no connections are present to be accepted"
				 ".")
	FAIL_SWITCH_ERRNO_DEFAULT_CASE()
	}
}

inline void
accept_handle(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address,
	      Handler *const handle,
	      void *arg)
{
	const char *restrict failure;

	if (LIKELY(accept_report(connect_descriptor,
				 socket_descriptor,
				 address,
				 length_address,
				 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
accept_handle_cl(int *const restrict connect_descriptor,
		 const int socket_descriptor,
		 struct sockaddr *const restrict address,
		 socklen_t *const restrict length_address,
		 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(accept_report(connect_descriptor,
				 socket_descriptor,
				 address,
				 length_address,
				 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}




#ifdef WIN32
/* size_adapters_addresses */
inline bool
size_adapters_addresses_status(ULONG family,
			       ULONG flags,
			       PVOID reserved,
			       PULONG size_pointer)
{
	return size_adapters_addresses_imp(family,
					   flags,
					   reserved,
					   size_pointer) == ERROR_BUFFER_OVERFLOW;
}

inline void
size_adapters_addresses_muffle(ULONG family,
			       ULONG flags,
			       PVOID reserved,
			       PULONG size_pointer)
{
	(void) size_adapters_addresses_imp(family,
					   flags,
					   reserved,
					   size_pointer);
}

#undef	FAIL_SWITCH_ROUTINE
#define	FAIL_SWITCH_ROUTINE size_adapters_addresses_imp
#define FAIL_SWITCH_STATUS_SUCCESS ERROR_BUFFER_OVERFLOW
inline bool
size_adapters_addresses_report(ULONG family,
			       ULONG flags,
			       PVOID reserved,
			       PULONG size_pointer,
			       const char *restrict *const restrict failure)
{
	FAIL_SWITCH_STATUS_OPEN(family,
				flags,
				reserved,
				size_pointer)
	FAIL_SWITCH_STATUS_CASE_1(ERROR_ADDRESS_NOT_ASSOCIATED,
				  "an address has not yet been associated with "
				  "the network endpoint  (DHCP lease "
				  "information was available)")
	FAIL_SWITCH_STATUS_CASE_3(ERROR_INVALID_PARAMETER,
				  "The 'size_pointer' parameter is NULL.",
				  "The 'address' paramter is not 'AF_INET', '"
				  "AF_INET6', or 'AF_UNSPEC'",
				  "The address information for the parameters "
				  "requested is greater than 'ULONG_MAX'.")
	FAIL_SWITCH_STATUS_CASE_1(ERROR_NOT_ENOUGH_MEMORY,
				  "Insufficient memory resources are available "
				  "to complete the operation.")
	FAIL_SWITCH_STATUS_CASE_1(ERROR_NO_DATA,
				  "No addresses were found for the requested "
				  "parameters")
	FAIL_SWITCH_STATUS_CLOSE()
}

inline void
size_adapters_addresses_handle(ULONG family,
			       ULONG flags,
			       PVOID reserved,
			       PULONG size_pointer,
			       Handler *const handle,
			       void *arg)
{
	const char *restrict failure;

	if (LIKELY(size_adapters_addresses_report(family,
						  flags,
						  reserved,
						  size_pointer,
						  &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
size_adapters_addresses_handle_cl(ULONG family,
				  ULONG flags,
				  PVOID reserved,
				  PULONG size_pointer,
				  const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(size_adapters_addresses_report(family,
						  flags,
						  reserved,
						  size_pointer,
						  &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* get_adapters_addresses */
inline bool
get_adapters_addresses_status(ULONG family,
			      ULONG flags,
			      PVOID reserved,
			      PIP_ADAPTER_ADDRESSES adapter_addresses,
			      PULONG size_pointer)
{
	return get_hardware_address_imp(family,
					flags,
					reserved,
					adapter_addresses,
					size_pointer) == ERROR_SUCCESS;
}

inline void
get_adapters_addresses_muffle(ULONG family,
			      ULONG flags,
			      PVOID reserved,
			      PIP_ADAPTER_ADDRESSES adapter_addresses,
			      PULONG size_pointer)
{
	(void) get_hardware_address_imp(family,
					flags,
					reserved,
					adapter_addresses,
					size_pointer);
}

#undef	FAIL_SWITCH_ROUTINE
#define	FAIL_SWITCH_ROUTINE get_adapters_addresses_imp
#undef	FAIL_SWITCH_STATUS_SUCCESS
#define FAIL_SWITCH_STATUS_SUCCESS ERROR_SUCCESS
inline bool
get_adapters_addresses_report(ULONG family,
			      ULONG flags,
			      PVOID reserved,
			      PIP_ADAPTER_ADDRESSES adapter_addresses,
			      PULONG size_pointer,
			      const char *restrict *const restrict failure)
{
	FAIL_SWITCH_STATUS_OPEN(family,
				flags,
				reserved,
				adapter_addresses,
				size_pointer)
	FAIL_SWITCH_STATUS_CASE_1(ERROR_ADDRESS_NOT_ASSOCIATED,
				  "an address has not yet been associated with "
				  "the network endpoint  (DHCP lease "
				  "information was available)")
	FAIL_SWITCH_STATUS_CASE_2(ERROR_BUFFER_OVERFLOW,
				 "The buffer size indicated by the '"
				 "size_pointer' parameter is too small to hold "
				 "the adapter information.",
				 "The 'adapter_addresses' parameter is 'NULL'.")
	FAIL_SWITCH_STATUS_CASE_3(ERROR_INVALID_PARAMETER,
				  "The 'size_pointer' parameter is NULL.",
				  "The 'address' paramter is not 'AF_INET', '"
				  "AF_INET6', or 'AF_UNSPEC'",
				  "The address information for the parameters "
				  "requested is greater than 'ULONG_MAX'.")
	FAIL_SWITCH_STATUS_CASE_1(ERROR_NOT_ENOUGH_MEMORY,
				  "Insufficient memory resources are available "
				  "to complete the operation.")
	FAIL_SWITCH_STATUS_CASE_1(ERROR_NO_DATA,
				  "No addresses were found for the requested "
				  "parameters")
	FAIL_SWITCH_STATUS_CLOSE()
}

inline void
get_adapters_addresses_handle(ULONG family,
			      ULONG flags,
			      PVOID reserved,
			      PIP_ADAPTER_ADDRESSES adapter_addresses,
			      PULONG size_pointer,
			      Handler *const handle,
			      void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_adapters_addresses_report(family,
						 flags,
						 reserved,
						 adapter_addresses,
						 size_pointer,
						 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_adapters_addresses_handle_cl(ULONG family,
				 ULONG flags,
				 PVOID reserved,
				 PIP_ADAPTER_ADDRESSES adapter_addresses,
				 PULONG size_pointer,
				 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_adapters_addresses_report(family,
						 flags,
						 reserved,
						 adapter_addresses,
						 size_pointer,
						 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}
#else


/* interface_name_to_index */
inline bool
interface_name_to_index_status(int *const restrict index,
			       const char *const restrict name)
{
	*index = interface_name_to_index_imp(name);
	return *index != 0;
}

inline void
interface_name_to_index_muffle(int *const restrict index,
			       const char *const restrict name)
{
	*index = interface_name_to_index_imp(name);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE interface_name_to_index_imp
inline bool
interface_name_to_index_report(int *const restrict index,
			       const char *const restrict name,
			       const char *restrict *const restrict failure)
{
	*index = interface_name_to_index_imp(name);

	if (LIKELY(*index != 0))
		return true;

	switch (errno) {
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "sysctl failure - "
				 "The buffer 'mib_name', 'old_data', 'new_data'"
				 ", or length pointer 'size_old_data' contains "
				 "an invalid address.")
	FAIL_SWITCH_ERRNO_CASE_3(EINVAL,
				 "sysctl failure - "
				 "The 'mib_name' array is less than two or "
				 "greater than 'CTL_MAXNAME'.",
				 "sysctl failure - "
				 "A non-null 'new_data' is given and its "
				 "specified length in 'size_new_data' is too "
				 "large or too small.",
				 "ioctl failure - "
				 "'configuration' or ioctl request is not valid"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_4(ENOMEM,
				 "sysctl failure - "
				 "The length pointed to by 'size_old_data' is "
				 "too short to hold the requested value.",
				 "sysctl failure - "
				 "The smaller of either the length pointed to "
				 "by 'size_old_data' or the estimated size of "
				 "the returned data exceeds the system limit on"
				 " locked memory.",
				 "sysctl failure - "
				 "Locking the buffer 'old_data', or a portion "
				 "of the buffer if the estimated size of the "
				 "data to be returned is smaller, would cause "
				 "the process to exceed its per-process locked "
				 "memory limit.",
				 "socket failure - "
				 "Insufficient memory was available to fulfill "
				 "the request.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTDIR,
				 "sysctl failure - "
				 "The 'mib_name' array specifies an "
				 "intermediate rather than terminal name.")
	FAIL_SWITCH_ERRNO_CASE_1(EISDIR,
				 "sysctl failure - "
				 "The 'mib_name' array specifies a terminal "
				 "name, but the actual name is not terminal.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOENT,
				 "sysctl failure - "
				 "The 'mib_name' array specifies a value that "
				 "is unknown.")
	FAIL_SWITCH_ERRNO_CASE_2(EPERM,
				 "sysctl failure - "
				 "An attempt is made to set a read-only value.",
				 "sysctl failure - "
				 "A process without appropriate privilege "
				 "attempts to set a value.")
	FAIL_SWITCH_ERRNO_CASE_1(EACCES,
				 "socket failure - "
				 "Permission to create a socket of the "
				 "specified type and/or protocol is denied.")
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "socket failure - "
				 "The specified address family is not supported"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(EMFILE,
				 "socket failure - "
				 "The per-process descriptor table is full.")
	FAIL_SWITCH_ERRNO_CASE_1(ENFILE,
				 "socket failure - "
				 "The system file table is full.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOBUFS,
				 "socket failure - "
				 "Insufficient buffer space is available. The "
				 "socket cannot be created until sufficient "
				 "resources are freed.")
	FAIL_SWITCH_ERRNO_CASE_1(EPROTONOSUPPORT,
				 "socket failure - "
				 "The protocol type or the specified protocol "
				 "is not supported within this domain.")
	FAIL_SWITCH_ERRNO_CASE_1(EPROTOTYPE,
				 "socket failure - "
				 "The socket type is not supported by the "
				 "protocol.")
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "ioctl failure - "
				 "'device_descriptor', is not a valid "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "ioctl failure - "
				 "'device_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl configuration 'SIOCGIFCONF' "
				 "does not apply to the kind of object that the"
				 " descriptor 'device_descriptor' references.")
	FAIL_SWITCH_ERRNO_CASE_1(ENXIO,
				 "no interface found for 'name'")
	FAIL_SWITCH_ERRNO_DEFAULT_CASE()
	}
}

inline void
interface_name_to_index_handle(int *const restrict index,
			       const char *const restrict name,
			       Handler *const handle,
			       void *arg)
{
	const char *restrict failure;

	if (LIKELY(interface_name_to_index_report(index,
						  name,
						  &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
interface_name_to_index_handle_cl(int *const restrict index,
				  const char *const restrict name,
				  const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(interface_name_to_index_report(index,
						  name,
						  &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}



/* get_interface_networks */
inline bool
get_interface_networks_status(struct ifconf *const restrict configuration,
			      const int device_descriptor)
{
	return get_interface_networks_imp(configuration,
					  device_descriptor) != -1;
}

inline void
get_interface_networks_muffle(struct ifconf *const restrict configuration,
			      const int device_descriptor)
{
	(void) get_interface_networks_imp(configuration,
					  device_descriptor);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE get_interface_networks_imp
inline bool
get_interface_networks_report(struct ifconf *const restrict configuration,
			      const int device_descriptor,
			      const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(configuration,
			       device_descriptor)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'device_descriptor', is not a valid "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'configuration' or ioctl request 'SIOCGIFCONF' is"
				 " not valid.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "'device_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl configuration 'SIOCGIFCONF' "
				 "does not apply to the kind of object that the"
				 " descriptor 'device_descriptor' references.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
get_interface_networks_handle(struct ifconf *const restrict configuration,
			      const int device_descriptor,
			      Handler *const handle,
			      void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_interface_networks_report(configuration,
						 device_descriptor,
						 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_interface_networks_handle_cl(struct ifconf *const restrict configuration,
				 const int device_descriptor,
				 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_interface_networks_report(configuration,
						 device_descriptor,
						 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* get_device_active_flags */
inline bool
get_device_active_flags_status(struct ifreq *const restrict request,
			       const int device_descriptor)
{
	return get_device_active_flags_imp(request,
					   device_descriptor) != -1;
}

inline void
get_device_active_flags_muffle(struct ifreq *const restrict request,
			       const int device_descriptor)
{
	(void) get_device_active_flags_imp(request,
					   device_descriptor);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE get_device_active_flags_imp
inline bool
get_device_active_flags_report(struct ifreq *const restrict request,
			       const int device_descriptor,
			       const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(request,
			       device_descriptor)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'device_descriptor', is not a valid "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'request' or ioctl request 'SIOCGIFFLAGS' is"
				 " not valid.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "'device_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl request 'SIOCGIFFLAGS' "
				 "does not apply to the kind of object that the"
				 " descriptor 'device_descriptor' references.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
get_device_active_flags_handle(struct ifreq *const restrict request,
			       const int device_descriptor,
			       Handler *const handle,
			       void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_device_active_flags_report(request,
						  device_descriptor,
						  &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_device_active_flags_handle_cl(struct ifreq *const restrict request,
				  const int device_descriptor,
				  const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_device_active_flags_report(request,
						  device_descriptor,
						  &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* get_winsize */
inline bool
get_winsize_status(struct winsize *const restrict window,
		   const int file_descriptor)
{
	return get_winsize_imp(window,
			       file_descriptor) != -1;
}

inline void
get_winsize_muffle(struct winsize *const restrict window,
		   const int file_descriptor)
{
	(void) get_winsize_imp(window,
			       file_descriptor);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE get_winsize_imp
inline bool
get_winsize_report(struct winsize *const restrict window,
		   const int file_descriptor,
		   const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(window,
			       file_descriptor)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'file_descriptor', is not a valid descriptor"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'window' or ioctl request 'TIOCGWINSZ' is not"
				 " valid.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "'file_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl request 'TIOCGWINSZ' does"
				 "not apply to the kind of object that the"
				 " descriptor 'file_descriptor' references.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
get_winsize_handle(struct winsize *const restrict window,
		   const int file_descriptor,
		   Handler *const handle,
		   void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_winsize_report(window,
				      file_descriptor,
				      &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_winsize_handle_cl(struct winsize *const restrict window,
		      const int file_descriptor,
		      const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_winsize_report(window,
				      file_descriptor,
				      &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}
#endif /* ifdef WIN32 */


#ifdef LINUX
/* get_interface_index */
inline bool
get_interface_index_status(struct ifreq *const restrict request,
			   const int device_descriptor)
{
	return get_interface_index_imp(request,
				       device_descriptor) != -1;
}

inline void
get_interface_index_muffle(struct ifreq *const restrict request,
			   const int device_descriptor)
{
	(void) get_interface_index_imp(request,
				       device_descriptor);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE get_interface_index_imp
inline bool
get_interface_index_report(struct ifreq *const restrict request,
			   const int device_descriptor,
			   const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(request,
			       device_descriptor)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'device_descriptor', is not a valid "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'request' or ioctl request 'SIOCGIFINDEX' is"
				 " not valid.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "'device_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl request 'SIOCGIFINDEX' "
				 "does not apply to the kind of object that the"
				 " descriptor 'device_descriptor' references.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
get_interface_index_handle(struct ifreq *const restrict request,
			   const int device_descriptor,
			   Handler *const handle,
			   void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_interface_index_report(request,
					      device_descriptor,
					      &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_interface_index_handle_cl(struct ifreq *const restrict request,
			      const int device_descriptor,
			      const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_interface_index_report(request,
					      device_descriptor,
					      &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* get_hardware_address */
inline bool
get_hardware_address_status(struct ifreq *const restrict request,
			    const int device_descriptor)
{
	return get_hardware_address_imp(request,
					device_descriptor) != -1;
}

inline void
get_hardware_address_muffle(struct ifreq *const restrict request,
			    const int device_descriptor)
{
	(void) get_hardware_address_imp(request,
					device_descriptor);
}

#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE get_hardware_address_imp
inline bool
get_hardware_address_report(struct ifreq *const restrict request,
			    const int device_descriptor,
			    const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(request,
			       device_descriptor)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "'device_descriptor', is not a valid "
				 "descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'request' or ioctl request 'SIOCGIFHWADDR' is"
				 " not valid.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTTY,
				 "'device_descriptor' is not associated with a "
				 "character special device.",
				 "The specified iotcl request 'SIOCGIFHWADDR' "
				 "does not apply to the kind of object that the"
				 " descriptor 'device_descriptor' references.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
get_hardware_address_handle(struct ifreq *const restrict request,
			    const int device_descriptor,
			    Handler *const handle,
			    void *arg)
{
	const char *restrict failure;

	if (LIKELY(get_hardware_address_report(request,
					       device_descriptor,
					       &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
get_hardware_address_handle_cl(struct ifreq *const restrict request,
			       const int device_descriptor,
			       const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(get_hardware_address_report(request,
					       device_descriptor,
					       &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}
#endif /* ifdef LINUX */

#ifdef OSX
/* sysctl */
inline bool
sysctl_status(int *const restrict mib_name,
	      u_int length_name,
	      void *const restrict old_data,
	      size_t *const restrict size_old_data,
	      void *const restrict new_data,
	      const size_t size_new_data)
{
	return sysctl(mib_name,
		      length_name,
		      old_data,
		      size_old_data,
		      new_data,
		      size_new_data) == 0;
}

inline void
sysctl_muffle(int *const restrict mib_name,
	      u_int length_name,
	      void *const restrict old_data,
	      size_t *const restrict size_old_data,
	      void *const restrict new_data,
	      const size_t size_new_data)
{
	(void) sysctl(mib_name,
		      length_name,
		      old_data,
		      size_old_data,
		      new_data,
		      size_new_data);
}


#undef  FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE sysctl
inline bool
sysctl_report(int *const restrict mib_name,
	      u_int length_name,
	      void *const restrict old_data,
	      size_t *const restrict size_old_data,
	      void *const restrict new_data,
	      const size_t size_new_data,
	      const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(mib_name,
			       length_name,
			       old_data,
			       size_old_data,
			       new_data,
			       size_new_data)
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The buffer 'mib_name', 'old_data', 'new_data'"
				 ", or length pointer 'size_old_data' contains "
				 "an invalid address.")
	FAIL_SWITCH_ERRNO_CASE_2(EINVAL,
				 "The 'mib_name' array is less than two or "
				 "greater than 'CTL_MAXNAME'.",
				 "A non-null 'new_data' is given and its "
				 "specified length in 'size_new_data' is too "
				 "large or too small.")
	FAIL_SWITCH_ERRNO_CASE_3(ENOMEM,
				 "The length pointed to by 'size_old_data' is "
				 "too short to hold the requested value.",
				 "The smaller of either the length pointed to "
				 "by 'size_old_data' or the estimated size of "
				 "the returned data exceeds the system limit on"
				 " locked memory.",
				 "Locking the buffer 'old_data', or a portion "
				 "of the buffer if the estimated size of the "
				 "data to be returned is smaller, would cause "
				 "the process to exceed its per-process locked "
				 "memory limit.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTDIR,
				 "The 'mib_name' array specifies an "
				 "intermediate rather than terminal name.")
	FAIL_SWITCH_ERRNO_CASE_1(EISDIR,
				 "The 'mib_name' array specifies a terminal "
				 "name, but the actual name is not terminal.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOENT,
				 "The 'mib_name' array specifies a value that "
				 "is unknown.")
	FAIL_SWITCH_ERRNO_CASE_2(EPERM,
				 "An attempt is made to set a read-only value.",
				 "A process without appropriate privilege "
				 "attempts to set a value.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
sysctl_handle(int *const restrict mib_name,
	      u_int length_name,
	      void *const restrict old_data,
	      size_t *const restrict size_old_data,
	      void *const restrict new_data,
	      const size_t size_new_data,
	      Handler *const handle,
	      void *arg)
{
	const char *restrict failure;

	if (LIKELY(sysctl_report(mib_name,
				 length_name,
				 old_data,
				 size_old_data,
				 new_data,
				 size_new_data,
				 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
sysctl_handle_cl(int *const restrict mib_name,
		 u_int length_name,
		 void *const restrict old_data,
		 size_t *const restrict size_old_data,
		 void *const restrict new_data,
		 const size_t size_new_data,
		 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(sysctl_report(mib_name,
				 length_name,
				 old_data,
				 size_old_data,
				 new_data,
				 size_new_data,
				 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}
#endif /* ifdef OSX */

#ifndef WIN32
/* getaddrinfo */
inline bool
getaddrinfo_status(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result)
{
	return getaddrinfo(node,
			   service,
			   hints,
			   result) == 0;
}

inline void
getaddrinfo_muffle(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result)
{
	(void) getaddrinfo(node,
			   service,
			   hints,
			   result);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE getaddrinfo
#undef	FAIL_SWITCH_STATUS_SUCCESS
#define FAIL_SWITCH_STATUS_SUCCESS 0

inline bool
getaddrinfo_report(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result,
		   const char *restrict *const restrict failure)
{
	FAIL_SWITCH_STATUS_OPEN(node,
				service,
				hints,
				result)
	FAIL_SWITCH_STATUS_CASE_1(EAI_ADDRFAMILY,
				  "The specified network host does not have any"
				  " network addresses in the requested address "
				  "family.")
	FAIL_SWITCH_STATUS_CASE_1(EAI_AGAIN,
				  "temporary failure in name resolution")
	FAIL_SWITCH_STATUS_CASE_1(EAI_BADFLAGS,
				  "invalid value for 'hints->ai_flags'")
	FAIL_SWITCH_STATUS_CASE_1(EAI_FAIL,
				  "non-recoverable failure in name resolution")
	FAIL_SWITCH_STATUS_CASE_1(EAI_FAMILY,
				  "ai_family not supported")
	FAIL_SWITCH_STATUS_CASE_1(EAI_MEMORY,
				  "memory allocation failure")
	FAIL_SWITCH_STATUS_CASE_1(EAI_NODATA,
				  "The specified network host exists, but it "
				  "does not have any network addresses defined"
				  ".")
	FAIL_SWITCH_STATUS_CASE_3(EAI_NONAME,
				  "The node or service is not known",
				  "both 'node' and 'service' are NULL",
				  "'AI_NUMERICSERV' was specified in "
				  "'hints->ai_flags' and 'service' was not a "
				  "numeric port-number string")
	FAIL_SWITCH_STATUS_CASE_1(EAI_SERVICE,
				  "'service' not supported for "
				  "'hints->ai_socktype'")
	FAIL_SWITCH_STATUS_CASE_1(EAI_SOCKTYPE,
				  "'hints->ai_socktype' not supported")
	FAIL_SWITCH_STATUS_CASE_1(EAI_SYSTEM,
				  "system error")
#ifdef EAI_BADHINTS
	FAIL_SWITCH_STATUS_CASE_1(EAI_BADHINTS,
				  "invalid value for 'hints'")
#endif /* ifdef EAI_BADHINTS */
#ifdef EAI_PROTOCOL
	FAIL_SWITCH_STATUS_CASE_1(EAI_PROTOCOL,
				  "resolved protocol is unknown")
#endif /* ifdef EAI_PROTOCOL */
#ifdef EAI_OVERFLOW
	FAIL_SWITCH_STATUS_CASE_1(EAI_OVERFLOW,
				  "argument buffer overflow")
#endif /* ifdef EAI_OVERFLOW */
	FAIL_SWITCH_STATUS_CLOSE()

}

inline void
getaddrinfo_handle(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result,
		   Handler *const handle,
		   void *arg)
{
	const char *restrict failure;

	if (LIKELY(getaddrinfo_report(node,
				      service,
				      hints,
				      result,
				      &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
getaddrinfo_handle_cl(const char *const node,
		      const char *const service,
		      const struct addrinfo *const hints,
		      struct addrinfo **const result,
		      const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(getaddrinfo_report(node,
				      service,
				      hints,
				      result,
				      &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}
#endif /* ifndef WIN32 */

/* signal */
inline bool
signal_status(sig_t *const restrict last,
	      const int name,
	      const sig_t action)
{
	*last = signal(name,
		       action);

	return *last != SIG_ERR;
}

inline void
signal_muffle(sig_t *const restrict last,
	      const int name,
	      const sig_t action)
{
	*last = signal(name,
		       action);
}

inline bool
signal_report(sig_t *const restrict last,
	      const int name,
	      const sig_t action,
	      const char *restrict *const restrict failure)
{
	*last = signal(name,
		       action);

	const bool success = (*last != SIG_ERR);

	if (UNLIKELY(!success))
		*failure = FAILURE_REASONS_2("signal",
					     "'name' is not a valid signal "
					     "number.",
					     "An attempt was made to ignore or "
					     "supply a handler for 'SIGKILL' or"
					     " 'SIGSTOP'.");

	return success;
}

inline void
signal_handle(sig_t *const restrict last,
	      const int name,
	      const sig_t action,
	      Handler *const handle,
	      void *arg)
{
	const char *restrict failure;

	if (LIKELY(signal_report(last,
				 name,
				 action,
				 &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
signal_handle_cl(sig_t *const restrict last,
		 const int name,
		 const sig_t action,
		 const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(signal_report(last,
				 name,
				 action,
				 &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* raise */
inline bool
raise_status(const int signal_name)
{
	return raise(signal_name) == 0;
}

inline void
raise_muffle(const int signal_name)
{
	(void) raise(signal_name);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE raise
inline bool
raise_report(const int signal_name,
	     const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(signal_name)
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'signal_name' is not a valid, supported "
				 "signal number.")
	FAIL_SWITCH_ERRNO_CASE_1(EPERM,
				 "The sending process is not the super-user and"
				 " its effective user id does not match the "
				 "effective user-id of the receiving process. "
				 " When signaling a process group, this error "
				 "is returned if any members of the group could"
				 " not be signaled.")
	FAIL_SWITCH_ERRNO_CASE_2(ESRCH,
				 "No process or process group could be found "
				 "corresponding to the process id returned by '"
				 "getpid'.",
				 "The process id was given as '0', but the "
				 "sending process does not have a process group"
				 ".")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
raise_handle(const int signal_name,
	     Handler *const handle,
	     void *arg)
{
	const char *restrict failure;

	if (LIKELY(raise_report(signal_name,
				&failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
raise_handle_cl(const int signal_name,
		const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(raise_report(signal_name,
				&failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
 * TOP-LEVEL FUNCTIONS */

/* undefine FAIL_SWITCH macro constants */
#undef FAIL_SWITCH_ROUTINE
#undef FAIL_SWITCH_ERRNO_FAILURE
#undef FAIL_SWITCH_FAILURE_POINTER

#endif /* ifndef MYSQL_SEED_SYSTEM_SYSTEM_UTILS_H_ */
