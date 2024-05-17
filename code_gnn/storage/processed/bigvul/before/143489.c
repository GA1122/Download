void CompositorImpl::DidCommit() {
  root_window_->OnCompositingDidCommit();
}
