void RenderView::AcceleratedSurfaceBuffersSwapped(
    gfx::PluginWindowHandle window, uint64 surface_id) {
  Send(new ViewHostMsg_AcceleratedSurfaceBuffersSwapped(
      routing_id(), window, surface_id));
}
