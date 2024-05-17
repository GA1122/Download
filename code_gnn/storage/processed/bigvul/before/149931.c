void LayerTreeHostImpl::DidChangeScrollbarVisibility() {
  client_->SetNeedsCommitOnImplThread();
}
