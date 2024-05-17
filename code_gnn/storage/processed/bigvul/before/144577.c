bool WebContentsImpl::IsWaitingForResponse() const {
  return waiting_for_response_ && is_load_to_different_document_;
}
