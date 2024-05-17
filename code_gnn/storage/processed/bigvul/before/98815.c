void WebPluginDelegateProxy::SetContainerVisibility(bool is_visible) {
  IPC::Message* msg;
  if (is_visible) {
    gfx::Rect window_frame = render_view_->rootWindowRect();
    gfx::Rect view_frame = render_view_->windowRect();
    WebKit::WebView* webview = render_view_->webview();
    msg = new PluginMsg_ContainerShown(instance_id_, window_frame, view_frame,
                                       webview && webview->isActive());
  } else {
    msg = new PluginMsg_ContainerHidden(instance_id_);
  }
  msg->set_unblock(true);
  Send(msg);
}
