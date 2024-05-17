void RenderViewImpl::AcceleratedSurfaceBuffersSwapped(
    gfx::PluginWindowHandle window, uint64 surface_handle) {
  Send(new ViewHostMsg_AcceleratedSurfaceBuffersSwapped(
      routing_id(), window, surface_handle));
}
