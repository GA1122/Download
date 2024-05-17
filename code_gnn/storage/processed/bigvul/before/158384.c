void RenderWidgetHostImpl::UnlockKeyboard() {
  if (IsKeyboardLocked())
    view_->UnlockKeyboard();
}
