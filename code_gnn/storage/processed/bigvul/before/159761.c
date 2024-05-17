void PermissionPromptImpl::Accept() {
  if (delegate_)
    delegate_->Accept();
}
