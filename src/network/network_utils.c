#include "network/network_utils.h"

/* socket */
extern inline bool
socket_status(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol);
extern inline void
socket_muffle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol);
extern inline bool
socket_report(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      const char *restrict *const restrict failure);
extern inline void
socket_handle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      Handler *const handle,
	      void *arg);
extern inline void
socket_handle_cl(int *const restrict socket_descriptor,
		 const int domain,
		 const int type,
		 const int protocol,
		 const struct HandlerClosure *const restrict fail_cl);

/* bind */
extern inline bool
bind_status(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address);
extern inline void
bind_muffle(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address);
extern inline bool
bind_report(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address,
	    const char *restrict *const restrict failure);
extern inline void
bind_handle(const int socket_descriptor,
	    const struct sockaddr *const restrict address,
	    const socklen_t length_address,
	    Handler *const handle,
	    void *arg);
extern inline void
bind_handle_cl(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       const struct HandlerClosure *const restrict fail_cl);

/* listen */
extern inline bool
listen_status(const int socket_descriptor,
	      const int backlog);
extern inline void
listen_muffle(const int socket_descriptor,
	      const int backlog);
extern inline bool
listen_report(const int socket_descriptor,
	      const int backlog,
	      const char *restrict *const restrict failure);
extern inline void
listen_handle(const int socket_descriptor,
	      const int backlog,
	      Handler *const handle,
	      void *arg);
extern inline void
listen_handle_cl(const int socket_descriptor,
		 const int backlog,
		 const struct HandlerClosure *const restrict fail_cl);

/* connect */
extern inline bool
connect_status(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address);
extern inline void
connect_muffle(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address);
extern inline bool
connect_report(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       const char *restrict *const restrict failure);
extern inline void
connect_handle(const int socket_descriptor,
	       const struct sockaddr *const restrict address,
	       const socklen_t length_address,
	       Handler *const handle,
	       void *arg);
extern inline void
connect_handle_cl(const int socket_descriptor,
		  const struct sockaddr *const restrict address,
		  const socklen_t length_address,
		  const struct HandlerClosure *const restrict fail_cl);

/* accept */
extern inline bool
accept_status(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address);
extern inline void
accept_muffle(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address);
extern inline bool
accept_report(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address,
	      const char *restrict *const restrict failure);
extern inline void
accept_handle(int *const restrict connect_descriptor,
	      const int socket_descriptor,
	      struct sockaddr *const restrict address,
	      socklen_t *const restrict length_address,
	      Handler *const handle,
	      void *arg);
extern inline void
accept_handle_cl(int *const restrict connect_descriptor,
		 const int socket_descriptor,
		 struct sockaddr *const restrict address,
		 socklen_t *const restrict length_address,
		 const struct HandlerClosure *const restrict fail_cl);

/* getsockname */
extern inline bool
getsockname_status(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address);
extern inline void
getsockname_muffle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address);
extern inline bool
getsockname_report(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   const char *restrict *const restrict failure);
extern inline void
getsockname_handle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   Handler *const handle,
		   void *arg);
extern inline void
getsockname_handle_cl(const int socket_descriptor,
		      struct sockaddr *const restrict address,
		      socklen_t *const restrict length_address,
		      const struct HandlerClosure *const restrict fail_cl);

/* getpeername */
extern inline bool
getpeername_status(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address);
extern inline void
getpeername_muffle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address);
extern inline bool
getpeername_report(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   const char *restrict *const restrict failure);
extern inline void
getpeername_handle(const int socket_descriptor,
		   struct sockaddr *const restrict address,
		   socklen_t *const restrict length_address,
		   Handler *const handle,
		   void *arg);
extern inline void
getpeername_handle_cl(const int socket_descriptor,
		      struct sockaddr *const restrict address,
		      socklen_t *const restrict length_address,
		      const struct HandlerClosure *const restrict fail_cl);

/* inet_pton */
extern inline bool
inet_pton_status(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address);
extern inline void
inet_pton_muffle(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address);
extern inline bool
inet_pton_report(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address,
		 const char *restrict *const restrict failure);
extern inline void
inet_pton_handle(const int address_family,
		 const char *const restrict presentation_format_address,
		 void *const restrict network_format_address,
		 Handler *const handle,
		 void *arg);
extern inline void
inet_pton_handle_cl(const int address_family,
		    const char *const restrict presentation_format_address,
		    void *const restrict network_format_address,
		    const struct HandlerClosure *const restrict fail_cl);

/* inet_ntop */
extern inline bool
inet_ntop_status(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer);
extern inline void
inet_ntop_muffle(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer);
extern inline bool
inet_ntop_report(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer,
		 const char *restrict *const restrict failure);
extern inline void
inet_ntop_handle(const int address_family,
		 const void *const restrict network_format_address,
		 char *const restrict presentation_format_address,
		 const socklen_t size_buffer,
		 Handler *const handle,
		 void *arg);
extern inline void
inet_ntop_handle_cl(const int address_family,
		    const void *const restrict network_format_address,
		    char *const restrict presentation_format_address,
		    const socklen_t size_buffer,
		    const struct HandlerClosure *const restrict fail_cl);
