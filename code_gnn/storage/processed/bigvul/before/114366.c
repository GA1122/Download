 void GpuChannel::OnCreateOffscreenCommandBuffer(
     const gfx::Size& size,
     const GPUCreateCommandBufferConfig& init_params,
    IPC::Message* reply_message) {
  int32 route_id = MSG_ROUTING_NONE;

  content::GetContentClient()->SetActiveURL(init_params.active_url);
#if defined(ENABLE_GPU)
  WillCreateCommandBuffer(init_params.gpu_preference);

  GpuCommandBufferStub* share_group = stubs_.Lookup(init_params.share_group_id);

  route_id = GenerateRouteID();

  scoped_ptr<GpuCommandBufferStub> stub(new GpuCommandBufferStub(
      this,
      share_group,
      gfx::GLSurfaceHandle(),
      size,
      disallowed_features_,
      init_params.allowed_extensions,
      init_params.attribs,
      init_params.gpu_preference,
      route_id,
      0, watchdog_,
      software_));
  router_.AddRoute(route_id, stub.get());
  stubs_.AddWithID(stub.release(), route_id);
  TRACE_EVENT1("gpu", "GpuChannel::OnCreateOffscreenCommandBuffer",
               "route_id", route_id);
#endif

  GpuChannelMsg_CreateOffscreenCommandBuffer::WriteReplyParams(
      reply_message,
      route_id);
  Send(reply_message);
}
