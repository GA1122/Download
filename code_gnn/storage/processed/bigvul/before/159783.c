void PermissionPromptImpl::TogglePersist(bool value) {
  if (delegate_)
    delegate_->TogglePersist(value);
}
