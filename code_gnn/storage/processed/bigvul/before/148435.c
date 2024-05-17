bool WebContentsImpl::IsLoadingToDifferentDocument() const {
  return IsLoading() && is_load_to_different_document_;
}
