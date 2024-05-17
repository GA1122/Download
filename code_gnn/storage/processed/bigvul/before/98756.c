CommandBufferProxy* WebPluginDelegateProxy::CreateCommandBuffer() {
#if defined(ENABLE_GPU)
#if defined(OS_MACOSX)
  if (!BindFakePluginWindowHandle(true))
    return NULL;
#endif
  int command_buffer_id;
  if (!Send(new PluginMsg_CreateCommandBuffer(instance_id_,
                                              &command_buffer_id))) {
    return NULL;
  }

  CommandBufferProxy* command_buffer =
      new CommandBufferProxy(channel_host_, command_buffer_id);
  channel_host_->AddRoute(command_buffer_id, command_buffer, NULL);
  return command_buffer;
#else
  return NULL;
#endif   
}
