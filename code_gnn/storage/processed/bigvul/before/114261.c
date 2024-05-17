bool GpuChannelHost::Send(IPC::Message* message) {
  message->set_unblock(false);

  if (factory_->IsMainThread()) {
    if (channel_.get())
      return channel_->Send(message);
  } else if (MessageLoop::current()) {
    return sync_filter_->Send(message);
  }

  delete message;
  return false;
}
