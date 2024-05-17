bool Channel::ChannelImpl::CreatePipe(const std::string& channel_id,
                                      Mode mode) {
  DCHECK(server_listen_pipe_ == -1 && pipe_ == -1);

  if (uses_fifo_) {
    pipe_name_ = "/var/tmp/chrome_" + channel_id;
    if (mode == MODE_SERVER) {
      if (!CreateServerFifo(pipe_name_, &server_listen_pipe_)) {
        return false;
      }
    } else {
      if (!ClientConnectToFifo(pipe_name_, &pipe_)) {
        return false;
      }
      waiting_connect_ = false;
    }
  } else {
    pipe_name_ = channel_id;
    pipe_ = ChannelNameToFD(pipe_name_);
    if (pipe_ < 0) {
      if (mode == MODE_SERVER) {
        if (!SocketPair(&pipe_, &client_pipe_))
          return false;
        AddChannelSocket(pipe_name_, client_pipe_);
      } else {
        static bool used_initial_channel = false;
        if (used_initial_channel) {
          LOG(FATAL) << "Denying attempt to reuse initial IPC channel for "
                     << pipe_name_;
          return false;
        }
        used_initial_channel = true;

        pipe_ = Singleton<base::GlobalDescriptors>()->Get(kPrimaryIPCChannel);
      }
    } else {
      waiting_connect_ = mode == MODE_SERVER;
    }
  }

  scoped_ptr<Message> msg(new Message(MSG_ROUTING_NONE,
                                      HELLO_MESSAGE_TYPE,
                                      IPC::Message::PRIORITY_NORMAL));
  #if !defined(OS_MACOSX)
  if (!uses_fifo_) {
    if (mode == MODE_SERVER) {
      fd_pipe_ = -1;
    } else if (remote_fd_pipe_ == -1) {
      if (!SocketPair(&fd_pipe_, &remote_fd_pipe_)) {
        return false;
      }
    }
  }
  #endif
  if (!msg->WriteInt(base::GetCurrentProcId())) {
    Close();
    return false;
  }

  output_queue_.push(msg.release());
  return true;
}
