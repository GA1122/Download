bool WebContentsImpl::CanOverscrollContent() const {
  if (force_disable_overscroll_content_)
    return false;

  if (delegate_)
    return delegate_->CanOverscrollContent();

  return false;
}
