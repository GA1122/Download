bool PermissionPromptImpl::CanAcceptRequestUpdate() {
  return !(bubble_delegate_ && bubble_delegate_->IsMouseHovered());
}
