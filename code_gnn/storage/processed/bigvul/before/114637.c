void WebPluginDelegateProxy::OnChannelError() {
  if (plugin_) {
    if (window_) {
      WillDestroyWindow();
    }
    plugin_->Invalidate();
  }
  if (!channel_host_->expecting_shutdown())
    render_view_->PluginCrashed(info_.path);

#if defined(OS_MACOSX) || defined(OS_WIN)
  if (render_view_)
    render_view_->PluginFocusChanged(false, instance_id_);
#endif
}
