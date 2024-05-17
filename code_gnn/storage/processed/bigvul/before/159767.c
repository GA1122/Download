void PermissionPromptImpl::Deny() {
  if (delegate_)
    delegate_->Deny();
}
