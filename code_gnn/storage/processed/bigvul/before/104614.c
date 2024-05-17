ExtensionPermissionMessages Extension::GetPermissionMessages() const {
  if (IsTrustedId(id_))
    return ExtensionPermissionMessages();
  else
    return permission_set_->GetPermissionMessages();
}
