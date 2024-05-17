int NetworkChangeNotifierLinux::Thread::ReadNotificationMessage(
    char* buf,
    size_t len) {
  DCHECK_NE(len, 0u);
  DCHECK(buf);
  memset(buf, 0, len);
  int rv = recv(netlink_fd_, buf, len, 0);
  if (rv > 0)
    return rv;

  DCHECK_NE(rv, 0);
  if (errno != EAGAIN && errno != EWOULDBLOCK) {
    PLOG(DFATAL) << "recv";
    return ERR_FAILED;
  }

   return ERR_IO_PENDING;
 }
