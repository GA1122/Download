PluginChannel* PluginChannel::GetPluginChannel(
    int renderer_id, base::MessageLoopProxy* ipc_message_loop) {
  std::string channel_key = StringPrintf(
      "%d.r%d", base::GetCurrentProcId(), renderer_id);

  PluginChannel* channel =
      static_cast<PluginChannel*>(NPChannelBase::GetChannel(
          channel_key,
          IPC::Channel::MODE_SERVER,
          ClassFactory,
          ipc_message_loop,
          false,
          ChildProcess::current()->GetShutDownEvent()));

  if (channel)
    channel->renderer_id_ = renderer_id;

  return channel;
}
