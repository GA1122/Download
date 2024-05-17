bool WebContentsImpl::IsBeingCaptured() const {
  return capturer_count_ > 0;
}
