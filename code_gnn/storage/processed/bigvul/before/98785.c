void WebPluginDelegateProxy::OnAcceleratedSurfaceSetIOSurface(
    gfx::PluginWindowHandle window,
    int32 width,
    int32 height,
    uint64 io_surface_identifier) {
  if (render_view_)
    render_view_->AcceleratedSurfaceSetIOSurface(window, width, height,
                                                 io_surface_identifier);
}
