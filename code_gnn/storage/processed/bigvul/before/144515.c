WebUI* WebContentsImpl::GetCommittedWebUI() const {
  return frame_tree_.root()->current_frame_host()->web_ui();
}
