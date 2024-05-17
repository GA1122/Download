void RenderViewImpl::UnregisterPluginDelegate(
    WebPluginDelegateProxy* delegate) {
  plugin_delegates_.erase(delegate);
}
