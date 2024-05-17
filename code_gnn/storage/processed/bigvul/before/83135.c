mrb_fd_cloexec(mrb_state *mrb, int fd)
{
#if defined(F_GETFD) && defined(F_SETFD) && defined(FD_CLOEXEC)
  int flags, flags2;

  flags = fcntl(fd, F_GETFD);
  if (flags == -1) {
    mrb_bug(mrb, "mrb_fd_cloexec: fcntl(%S, F_GETFD) failed: %S",
      mrb_fixnum_value(fd), mrb_fixnum_value(errno));
  }
  if (fd <= 2) {
    flags2 = flags & ~FD_CLOEXEC;  
  }
  else {
    flags2 = flags | FD_CLOEXEC;  
  }
  if (flags != flags2) {
    if (fcntl(fd, F_SETFD, flags2) == -1) {
      mrb_bug(mrb, "mrb_fd_cloexec: fcntl(%S, F_SETFD, %S) failed: %S",
        mrb_fixnum_value(fd), mrb_fixnum_value(flags2), mrb_fixnum_value(errno));
    }
  }
#endif
}
