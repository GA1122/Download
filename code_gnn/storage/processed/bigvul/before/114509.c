bool PluginChannel::Init(base::MessageLoopProxy* ipc_message_loop,
                         bool create_pipe_now,
                         base::WaitableEvent* shutdown_event) {
  if (!NPChannelBase::Init(ipc_message_loop, create_pipe_now, shutdown_event))
    return false;

  channel_->AddFilter(filter_.get());
  return true;
}
