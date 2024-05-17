bool WebContentsImpl::RequestKeyboardLock(
    RenderWidgetHostImpl* render_widget_host,
    bool esc_key_locked) {
  DCHECK(render_widget_host);
  if (render_widget_host->delegate()->GetAsWebContents() != this) {
    NOTREACHED();
    return false;
  }

  if (GetOuterWebContents())
    return false;

  esc_key_locked_ = esc_key_locked;
  keyboard_lock_widget_ = render_widget_host;

  if (delegate_)
    delegate_->RequestKeyboardLock(this, esc_key_locked_);
  return true;
}
