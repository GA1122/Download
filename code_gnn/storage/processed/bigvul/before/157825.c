void WebContentsImpl::OnScreenOrientationChange() {
  DCHECK(screen_orientation_provider_);
  screen_orientation_provider_->OnOrientationChange();
}
