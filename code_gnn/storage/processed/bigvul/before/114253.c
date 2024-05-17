void GpuChannelHost::DestroyCommandBuffer(
    CommandBufferProxy* command_buffer) {
#if defined(ENABLE_GPU)
  AutoLock lock(context_lock_);
  int route_id = command_buffer->GetRouteID();
  Send(new GpuChannelMsg_DestroyCommandBuffer(route_id));
  if (proxies_.find(route_id) != proxies_.end())
    proxies_.erase(route_id);
  RemoveRoute(route_id);
  delete command_buffer;
#endif
}
