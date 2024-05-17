void WebPluginProxy::AcceleratedFrameBuffersDidSwap(
    gfx::PluginWindowHandle window, uint64 surface_handle) {
  Send(new PluginHostMsg_AcceleratedSurfaceBuffersSwapped(
        route_id_, window, surface_handle));
}
