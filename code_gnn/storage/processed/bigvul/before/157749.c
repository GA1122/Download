void WebContentsImpl::DidCallFocus() {
  ForSecurityDropFullscreen();
}