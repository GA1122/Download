gfx::NativeWindow PermissionPromptImpl::GetNativeWindow() {
  if (bubble_delegate_ && bubble_delegate_->GetWidget())
    return bubble_delegate_->GetWidget()->GetNativeWindow();
  return nullptr;
}
