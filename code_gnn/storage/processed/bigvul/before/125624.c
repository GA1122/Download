bool RenderViewHostImpl::IsFullscreen() const {
  return delegate_->IsFullscreenForCurrentTab();
}
