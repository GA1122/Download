void WebPluginDelegateStub::OnCreateCommandBuffer(int* route_id) {
#if defined(ENABLE_GPU)
  command_buffer_stub_.reset(new CommandBufferStub(
      static_cast<PluginChannel*>(PluginChannelBase::GetCurrentChannel()),
      delegate_->windowed_handle()));

  *route_id = command_buffer_stub_->route_id();
#else
  *route_id = 0;
#endif   
}
