void WebContentsImpl::DidNavigateMainFramePreCommit(
    bool navigation_is_within_page) {
  if (navigation_is_within_page) {
    return;
  }
  if (IsFullscreenForCurrentTab())
    ExitFullscreen(false);
  DCHECK(!IsFullscreenForCurrentTab());
}
