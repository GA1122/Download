bool WebContentsImpl::IsNeverVisible() {
  if (!delegate_)
    return false;
  return delegate_->IsNeverVisible(this);
}
