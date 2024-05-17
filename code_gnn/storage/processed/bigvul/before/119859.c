void WebContentsImpl::ToggleFullscreenMode(bool enter_fullscreen) {
  if (delegate_)
    delegate_->ToggleFullscreenModeForTab(this, enter_fullscreen);
}
