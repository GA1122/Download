void Channel::ChannelImpl::Close() {

  server_listen_connection_watcher_.StopWatchingFileDescriptor();

  if (server_listen_pipe_ != -1) {
    HANDLE_EINTR(close(server_listen_pipe_));
    server_listen_pipe_ = -1;
  }

  read_watcher_.StopWatchingFileDescriptor();
  write_watcher_.StopWatchingFileDescriptor();
  if (pipe_ != -1) {
    HANDLE_EINTR(close(pipe_));
    pipe_ = -1;
  }
  if (client_pipe_ != -1) {
    Singleton<PipeMap>()->RemoveAndClose(pipe_name_);
    client_pipe_ = -1;
  }
#if !defined(OS_MACOSX)
  if (fd_pipe_ != -1) {
    HANDLE_EINTR(close(fd_pipe_));
    fd_pipe_ = -1;
  }
  if (remote_fd_pipe_ != -1) {
    HANDLE_EINTR(close(remote_fd_pipe_));
    remote_fd_pipe_ = -1;
  }
#endif

  if (uses_fifo_) {
    unlink(pipe_name_.c_str());
  }

  while (!output_queue_.empty()) {
    Message* m = output_queue_.front();
    output_queue_.pop();
    delete m;
  }

  for (std::vector<int>::iterator
       i = input_overflow_fds_.begin(); i != input_overflow_fds_.end(); ++i) {
    HANDLE_EINTR(close(*i));
  }
  input_overflow_fds_.clear();
}
