void PermissionPromptImpl::Closing() {
  if (bubble_delegate_)
    bubble_delegate_ = nullptr;
  if (delegate_)
    delegate_->Closing();
}
