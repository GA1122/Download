CommandBufferProxy* WebPluginDelegateProxy::CreateCommandBuffer() {
#if defined(ENABLE_GPU)
  int command_buffer_id;
  if (!Send(new PluginMsg_CreateCommandBuffer(instance_id_,
                                              &command_buffer_id))) {
    return NULL;
  }

  return new CommandBufferProxy(channel_host_, command_buffer_id);
#else
  return NULL;
#endif   
}
