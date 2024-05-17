void WebPluginDelegateProxy::OnChannelError() {
  if (plugin_) {
    if (window_) {
      WillDestroyWindow();
    }
    plugin_->Invalidate();
  }
  render_view_->PluginCrashed(info_.path);
}
