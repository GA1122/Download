void WebContentsImpl::DidAccessInitialDocument() {
  has_accessed_initial_document_ = true;

  if (!IsLoading() && controller_.GetPendingEntry())
    controller_.DiscardPendingEntry(false);

  NotifyNavigationStateChanged(INVALIDATE_TYPE_URL);
}
