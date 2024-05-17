void WebPluginDelegateProxy::OnAcceleratedPluginAllocatedIOSurface(
    int32 width,
    int32 height,
    uint32 surface_id) {
  if (plugin_)
    plugin_->AcceleratedPluginAllocatedIOSurface(width, height, surface_id);
}
