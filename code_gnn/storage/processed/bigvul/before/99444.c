bool SocketPair(int* fd1, int* fd2) {
  int pipe_fds[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_fds) != 0) {
    PLOG(ERROR) << "socketpair()";
    return false;
  }

  if (fcntl(pipe_fds[0], F_SETFL, O_NONBLOCK) == -1 ||
      fcntl(pipe_fds[1], F_SETFL, O_NONBLOCK) == -1) {
    PLOG(ERROR) << "fcntl(O_NONBLOCK)";
    HANDLE_EINTR(close(pipe_fds[0]));
    HANDLE_EINTR(close(pipe_fds[1]));
    return false;
  }

  *fd1 = pipe_fds[0];
  *fd2 = pipe_fds[1];

  return true;
}
