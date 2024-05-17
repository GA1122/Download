void RenderWidgetHostViewAura::AcceleratedSurfaceBuffersSwapped(
    const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params_in_pixel,
    int gpu_host_id) {
  BufferPresentedCallback ack_callback = base::Bind(
      &AcknowledgeBufferForGpu,
      params_in_pixel.route_id,
      gpu_host_id,
      params_in_pixel.mailbox_name);
  BuffersSwapped(params_in_pixel.size,
                 gfx::Rect(params_in_pixel.size),
                 params_in_pixel.scale_factor,
                 params_in_pixel.mailbox_name,
                 params_in_pixel.latency_info,
                 ack_callback);
}
