void WebPluginDelegateProxy::OnAcceleratedPluginSwappedIOSurface() {
  if (plugin_)
    plugin_->AcceleratedPluginSwappedIOSurface();
}
