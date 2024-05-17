void WebPluginProxy::SetAcceleratedSurface(
    gfx::PluginWindowHandle window,
    const gfx::Size& size,
    uint64 accelerated_surface_identifier) {
  Send(new PluginHostMsg_AcceleratedSurfaceSetIOSurface(
      route_id_, window, size.width(), size.height(),
      accelerated_surface_identifier));
}
