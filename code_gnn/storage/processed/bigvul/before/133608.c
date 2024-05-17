void WebContentsImpl::OnMoveValidationMessage(
    const gfx::Rect& anchor_in_root_view) {
  if (delegate_)
    delegate_->MoveValidationMessage(this, anchor_in_root_view);
}
