void WebContentsImpl::RequestToLockMouse(
    RenderWidgetHostImpl* render_widget_host,
    bool user_gesture,
    bool last_unlocked_by_target) {
  if (render_widget_host != GetRenderViewHost()->GetWidget()) {
    render_widget_host->GotResponseToLockMouseRequest(false);
    return;
  }

  if (delegate_)
    delegate_->RequestToLockMouse(this, user_gesture, last_unlocked_by_target);
  else
    GotResponseToLockMouseRequest(false);
}
