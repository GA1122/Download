void WebPluginDelegateProxy::OnStartIme() {
  if (render_view_)
    render_view_->StartPluginIme();
}
