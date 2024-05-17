void RenderWidgetHostViewAura::OnCaptureLost() {
  host_->LostCapture();
  if (touch_editing_client_)
    touch_editing_client_->EndTouchEditing(false);
}
