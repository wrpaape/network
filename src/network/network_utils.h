#ifndef NETWORK_NETWORK_NETWORK_UTILS_H_
#define NETWORK_NETWORK_NETWORK_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "system/exit_utils.h"	/* exit|file|system|string_utils */
#include "arpa/inet.h"		/* inet_* API */


/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
/* correct network byte order */
#if IS_LITTLE_ENDIAN
#	define NETWORK_SHORT(SHORT) __builtin_bswap16(SHORT)
#	define NETWORK_LONG(LONG)   __builtin_bswap32(LONG)
#else
#	define NETWORK_SHORT(SHORT) SHORT
#	define NETWORK_LONG(LONG)   LONG
#endif /* if IS_LITTLE_ENDIAN */

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


/* getsockname */
inline bool
getsockname_status(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address)
{
	return getsockname(socket_descriptor,
			   address,
			   length_address) == 0;
}

inline void
getsockname_muffle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address)
{
	(void) getsockname(socket_descriptor,
			   address,
			   length_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE getsockname
inline bool
getsockname_report(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(socket_descriptor,
			       address,
			       length_address)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "The argument 'socket_descriptor' is not a "
				 "valid file descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The address parameter points to memory not in"
				 " a valid part of the process address space.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'socket_descriptor' has been shut down.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOBUFS,
				 "Insufficient resources were available in the "
				 "system to perform the operation.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "The argument 'socket_descriptor' is not a "
				 "socket (e.g., a plain file).")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "getsockname() is not supported for the "
				 "protocol in use by 'socket_descriptor'.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
getsockname_handle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   Handler *const handle,
		   void *arg)
{
	const char *restrict failure;

	if (LIKELY(getsockname_report(socket_descriptor,
				      address,
				      length_address,
				      &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
getsockname_handle_cl(const int socket_descriptor,
		      struct sockaddr *const restrict address,
		      socklen_t *const restrict length_address,
		      const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(getsockname_report(socket_descriptor,
				      address,
				      length_address,
				      &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* getpeername */
inline bool
getpeername_status(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address)
{
	return getpeername(socket_descriptor,
			   address,
			   length_address) == 0;
}

inline void
getpeername_muffle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address)
{
	(void) getpeername(socket_descriptor,
			   address,
			   length_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE getpeername
inline bool
getpeername_report(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(socket_descriptor,
			       address,
			       length_address)
	FAIL_SWITCH_ERRNO_CASE_1(EBADF,
				 "The argument 'socket_descriptor' is not a "
				 "valid file descriptor.")
	FAIL_SWITCH_ERRNO_CASE_1(EFAULT,
				 "The address parameter points to memory not in"
				 " a valid part of the process address space.")
	FAIL_SWITCH_ERRNO_CASE_1(EINVAL,
				 "'socket_descriptor' has been shut down.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOBUFS,
				 "Insufficient resources were available in the "
				 "system to perform the operation.")
	FAIL_SWITCH_ERRNO_CASE_2(ENOTCONN,
				 "the socket is not connected",
				 "the socket has not had the peer pre-specified"
				 ".")
	FAIL_SWITCH_ERRNO_CASE_1(ENOTSOCK,
				 "The argument 'socket_descriptor' is not a "
				 "socket (e.g., a plain file).")
	FAIL_SWITCH_ERRNO_CASE_1(EOPNOTSUPP,
				 "getpeername() is not supported for the "
				 "protocol in use by 'socket_descriptor'.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
getpeername_handle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   Handler *const handle,
		   void *arg)
{
	const char *restrict failure;

	if (LIKELY(getpeername_report(socket_descriptor,
				      address,
				      length_address,
				      &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
getpeername_handle_cl(const int socket_descriptor,
		      struct sockaddr *const restrict address,
		      socklen_t *const restrict length_address,
		      const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(getpeername_report(socket_descriptor,
				      address,
				      length_address,
				      &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* inet_pton */
inline bool
inet_pton_status(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address)
{
	return inet_pton(address_family,
			 presentation_format_address,
			 network_format_address) == 1;
}

inline void
inet_pton_muffle(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address)
{
	(void) inet_pton(address_family,
			 presentation_format_address,
			 network_format_address);
}

#undef	FAIL_SWITCH_ROUTINE
#define FAIL_SWITCH_ROUTINE inet_pton
inline bool
inet_pton_report(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address,
		 const char *restrict *const restrict failure)
{
	const int status = inet_pton(address_family,
				     presentation_format_address,
				     network_format_address);

	if (LIKELY(status == 1))
		return true;

	if (status == 0) {
		*failure = FAILURE_REASON("inet_pton",
					  "'presentation_format_address' was "
					  "not paresable for the specified "
					  "address family, 'address_family'.");
		return false;
	}

	switch (errno) {
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "'address_family' does not conatin a valid "
				 "address family.")
	FAIL_SWITCH_ERRNO_DEFAULT_CASE()
	}
}

inline void
inet_pton_handle(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address,
		 Handler *const handle,
		 void *arg)
{
	const char *restrict failure;

	if (LIKELY(inet_pton_report(address_family,
				    presentation_format_address,
				    network_format_address,
				    &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
inet_pton_handle_cl(const int address_family,
		    const char *const restrict presentation_format_address,
		    void *const restrict network_format_address,
		    const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(inet_pton_report(address_family,
				    presentation_format_address,
				    network_format_address,
				    &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* inet_ntop */
inline bool
inet_ntop_status(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer)
{
	return inet_ntop(address_family,
			 network_format_address,
			 presentation_format_address,
			 size_buffer) != NULL;
}

inline void
inet_ntop_muffle(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer)
{
	(void) inet_ntop(address_family,
			 network_format_address,
			 presentation_format_address,
			 size_buffer);
}

#undef	FAIL_SWITCH_ROUTINE
#undef	FAIL_SWITCH_ERRNO_FAILURE
#define FAIL_SWITCH_ROUTINE		inet_ntop
#define	FAIL_SWITCH_ERRNO_FAILURE	NULL
inline bool
inet_ntop_report(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer,
		 const char *restrict *const restrict failure)
{
	FAIL_SWITCH_ERRNO_OPEN(address_family,
			       network_format_address,
			       presentation_format_address,
			       size_buffer)
	FAIL_SWITCH_ERRNO_CASE_1(EAFNOSUPPORT,
				 "'network_format_address' was not an 'AF_INET'"
				 " or 'AF_INET6' family address.")
	FAIL_SWITCH_ERRNO_CASE_1(ENOSPC,
				 "'size_buffer' was not large enough to store "
				 "the presentation form of the address, '"
				 "presentation_format_address'.")
	FAIL_SWITCH_ERRNO_CLOSE()
}

inline void
inet_ntop_handle(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer,
		 Handler *const handle,
		 void *arg)
{
	const char *restrict failure;

	if (LIKELY(inet_ntop_report(address_family,
				    network_format_address,
				    presentation_format_address,
				    size_buffer,
				    &failure)))
		return;

	handle(arg,
	       failure);
	__builtin_unreachable();
}

inline void
inet_ntop_handle_cl(const int address_family,
		    const void *const restrict network_format_address,
		    char *const restrict presentation_format_address,
		    const socklen_t size_buffer,
		    const struct HandlerClosure *const restrict fail_cl)
{
	const char *restrict failure;

	if (LIKELY(inet_ntop_report(address_family,
				    network_format_address,
				    presentation_format_address,
				    size_buffer,
				    &failure)))
		return;

	handler_closure_call(fail_cl,
			     failure);
	__builtin_unreachable();
}


/* undefine FAIL_SWITCH macro constants */
#undef FAIL_SWITCH_ROUTINE
#undef FAIL_SWITCH_ERRNO_FAILURE
#undef FAIL_SWITCH_FAILURE_POINTER

#endif /* ifndef NETWORK_NETWORK_NETWORK_UTILS_H_ */

