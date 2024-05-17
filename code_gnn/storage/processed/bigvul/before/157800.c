bool WebContentsImpl::IsHidden() {
  return !IsBeingCaptured() && visibility_ != Visibility::VISIBLE;
}
