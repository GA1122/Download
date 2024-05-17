void Channel::ChannelImpl::OnFileCanReadWithoutBlocking(int fd) {
  bool send_server_hello_msg = false;
  if (waiting_connect_ && mode_ == MODE_SERVER) {
    if (uses_fifo_) {
      if (!ServerAcceptFifoConnection(server_listen_pipe_, &pipe_)) {
        Close();
      }

      server_listen_connection_watcher_.StopWatchingFileDescriptor();

      MessageLoopForIO::current()->WatchFileDescriptor(
          pipe_,
          true,
          MessageLoopForIO::WATCH_READ,
          &read_watcher_,
          this);

      waiting_connect_ = false;
    } else {
      waiting_connect_ = false;
    }
    send_server_hello_msg = true;
  }

  if (!waiting_connect_ && fd == pipe_) {
    if (!ProcessIncomingMessages()) {
      Close();
      listener_->OnChannelError();
      return;
    }
  }

  if (send_server_hello_msg) {
    ProcessOutgoingMessages();
  }
}
