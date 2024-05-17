void WebContentsImpl::ExitFullscreen(bool will_cause_resize) {
  GetRenderViewHost()->GetWidget()->RejectMouseLockOrUnlockIfNecessary();
  ExitFullscreenMode(will_cause_resize);
}
