void WebContentsImpl::EnterFullscreenMode(
    const GURL& origin,
    const blink::WebFullscreenOptions& options) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view) {
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())
        ->ShutdownAndDestroyWidget(true);
  }

  if (delegate_) {
    delegate_->EnterFullscreenModeForTab(this, origin, options);

    if (keyboard_lock_widget_)
      delegate_->RequestKeyboardLock(this, esc_key_locked_);
  }

  for (auto& observer : observers_)
    observer.DidToggleFullscreenModeForTab(IsFullscreenForCurrentTab(), false);
}
