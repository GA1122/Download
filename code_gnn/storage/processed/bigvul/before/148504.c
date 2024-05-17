void WebContentsImpl::OnScreenOrientationChange() {
  DCHECK(screen_orientation_provider_);
  return screen_orientation_provider_->OnOrientationChange();
}
