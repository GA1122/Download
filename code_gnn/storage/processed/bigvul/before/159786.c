PermissionPromptImpl::~PermissionPromptImpl() {
  if (bubble_delegate_)
    bubble_delegate_->CloseBubble();
}
