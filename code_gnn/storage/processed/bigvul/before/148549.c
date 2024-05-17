void WebContentsImpl::SendPageMessage(IPC::Message* msg) {
  frame_tree_.root()->render_manager()->SendPageMessage(msg, nullptr);
}
