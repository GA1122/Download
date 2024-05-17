void RenderSandboxHostLinux::Init(const std::string& sandbox_path) {
  DCHECK(!init_);
  init_ = true;

  int fds[2];
  CHECK(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) == 0);

  renderer_socket_ = fds[0];
  const int browser_socket = fds[1];

  int pipefds[2];
  CHECK(0 == pipe(pipefds));
  const int child_lifeline_fd = pipefds[0];
  childs_lifeline_fd_ = pipefds[1];

  pid_ = fork();
  if (pid_ == 0) {
    SandboxIPCProcess handler(child_lifeline_fd, browser_socket, sandbox_path);
    handler.Run();
    _exit(0);
  }
}
