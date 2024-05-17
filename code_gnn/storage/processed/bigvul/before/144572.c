bool WebContentsImpl::IsLoadingToDifferentDocument() const {
  return is_loading_ && is_load_to_different_document_;
}
