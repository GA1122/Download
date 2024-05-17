int32 RenderThreadImpl::CreateViewCommandBuffer(
      int32 surface_id, const GPUCreateCommandBufferConfig& init_params) {
  int32 route_id = MSG_ROUTING_NONE;
  IPC::Message* message = new GpuHostMsg_CreateViewCommandBuffer(
      surface_id,
      init_params,
      &route_id);

  if (MessageLoop::current() == message_loop())
    ChildThread::Send(message);
  else
    sync_message_filter()->Send(message);

  return route_id;
}
