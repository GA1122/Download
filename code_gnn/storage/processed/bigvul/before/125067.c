void RenderProcessHostImpl::OnCompositorSurfaceBuffersSwappedNoHost(
      int32 surface_id,
      uint64 surface_handle,
      int32 route_id,
      const gfx::Size& size,
      int32 gpu_process_host_id) {
  TRACE_EVENT0("renderer_host",
               "RenderWidgetHostImpl::OnCompositorSurfaceBuffersSwappedNoHost");
  AcceleratedSurfaceMsg_BufferPresented_Params ack_params;
  ack_params.sync_point = 0;
  RenderWidgetHostImpl::AcknowledgeBufferPresent(route_id,
                                                 gpu_process_host_id,
                                                 ack_params);
}
