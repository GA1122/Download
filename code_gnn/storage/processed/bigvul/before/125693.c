void RenderViewHostImpl::OnToggleFullscreen(bool enter_fullscreen) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  delegate_->ToggleFullscreenMode(enter_fullscreen);
  WasResized();
}
