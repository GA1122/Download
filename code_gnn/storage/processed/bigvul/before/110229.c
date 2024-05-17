  ~NaClGdbWatchDelegate() {
    if (HANDLE_EINTR(close(fd_read_)) != 0)
      DLOG(ERROR) << "close(fd_read_) failed";
    if (HANDLE_EINTR(close(fd_write_)) != 0)
      DLOG(ERROR) << "close(fd_write_) failed";
  }
