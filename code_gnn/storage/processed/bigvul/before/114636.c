void WebPluginDelegateProxy::OnAcceleratedSurfaceBuffersSwapped(
    gfx::PluginWindowHandle window, uint64 surface_handle) {
  if (render_view_)
    render_view_->AcceleratedSurfaceBuffersSwapped(window, surface_handle);
}
