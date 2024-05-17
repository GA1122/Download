bool WebContentsImpl::IsHidden() {
  return !IsBeingCaptured() && !should_normally_be_visible_;
}
