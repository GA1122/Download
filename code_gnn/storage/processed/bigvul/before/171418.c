static int do_server() {
  signal(SIGABRT, SIG_DFL);
  signal(SIGBUS, SIG_DFL);
  signal(SIGFPE, SIG_DFL);
  signal(SIGILL, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
#ifdef SIGSTKFLT
  signal(SIGSTKFLT, SIG_DFL);
#endif
  signal(SIGTRAP, SIG_DFL);

  signal(SIGPIPE, SIG_IGN);

 sigset_t sigchld;
  sigemptyset(&sigchld);
  sigaddset(&sigchld, SIGCHLD);
  sigprocmask(SIG_SETMASK, &sigchld, nullptr);

 int s = socket_local_server(SOCKET_NAME, ANDROID_SOCKET_NAMESPACE_ABSTRACT,
                              SOCK_STREAM | SOCK_CLOEXEC);
 if (s == -1) return 1;

 if (!start_signal_sender()) {
    ALOGE("debuggerd: failed to fork signal sender");
 return 1;
 }

  ALOGI("debuggerd: starting\n");

 for (;;) {
    sockaddr_storage ss;
    sockaddr* addrp = reinterpret_cast<sockaddr*>(&ss);
 socklen_t alen = sizeof(ss);

    ALOGV("waiting for connection\n");
 int fd = accept4(s, addrp, &alen, SOCK_CLOEXEC);
 if (fd == -1) {
      ALOGE("accept failed: %s\n", strerror(errno));
 continue;
 }

    handle_request(fd);
 }
 return 0;
}
