void SetCloseOnExec(nacl::Handle fd) {
#if defined(OS_POSIX)
  int flags = fcntl(fd, F_GETFD);
  CHECK(flags != -1);
  int rc = fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
  CHECK(rc == 0);
#endif
}
