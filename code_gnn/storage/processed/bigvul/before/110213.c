  NaClGdbWatchDelegate(int fd_read, int fd_write,
                       const base::Closure& reply)
      : fd_read_(fd_read),
        fd_write_(fd_write),
        reply_(reply) {}
