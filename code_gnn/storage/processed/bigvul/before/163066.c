SandboxIPCHandler::~SandboxIPCHandler() {
  if (IGNORE_EINTR(close(lifeline_fd_)) < 0)
    PLOG(ERROR) << "close";
  if (IGNORE_EINTR(close(browser_socket_)) < 0)
    PLOG(ERROR) << "close";
}
