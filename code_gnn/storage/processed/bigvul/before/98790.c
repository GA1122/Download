void WebPluginDelegateProxy::OnChannelError() {
  if (plugin_) {
    if (window_) {
      WillDestroyWindow();
    }
    plugin_->Invalidate();
  }
  if (!channel_host_->expecting_shutdown())
    render_view_->PluginCrashed(info_.path);
}
