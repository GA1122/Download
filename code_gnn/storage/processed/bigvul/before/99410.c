  void Run() {
    struct pollfd pfds[2];
    pfds[0].fd = lifeline_fd_;
    pfds[0].events = POLLIN;
    pfds[1].fd = browser_socket_;
    pfds[1].events = POLLIN;

    int failed_polls = 0;
    for (;;) {
      const int r = HANDLE_EINTR(poll(pfds, 2, -1));
      if (r < 1) {
        LOG(WARNING) << "poll errno:" << errno;
        if (failed_polls++ == 3) {
          LOG(FATAL) << "poll failing. Sandbox host aborting.";
          return;
        }
        continue;
      }

      failed_polls = 0;

      if (pfds[0].revents) {
        _exit(0);
      }

      if (pfds[1].revents) {
        HandleRequestFromRenderer(browser_socket_);
      }
    }
  }
