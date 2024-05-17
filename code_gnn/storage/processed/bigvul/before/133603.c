void WebContentsImpl::OnJavaBridgeGetChannelHandle(IPC::Message* reply_msg) {
  java_bridge_dispatcher_host_manager_->OnGetChannelHandle(
      render_view_message_source_, reply_msg);
}
