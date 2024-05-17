void WebPluginDelegateProxy::OnFocusChanged(bool focused) {
  if (render_view_)
    render_view_->PluginFocusChanged(focused, instance_id_);
}
