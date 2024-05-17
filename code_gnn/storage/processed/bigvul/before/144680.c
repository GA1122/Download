bool WebContentsImpl::ShouldPreserveAbortedURLs() {
  if (!delegate_)
    return false;
  return delegate_->ShouldPreserveAbortedURLs(this);
}
