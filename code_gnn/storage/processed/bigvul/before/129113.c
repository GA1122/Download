PermissionMessages PermissionsData::GetPermissionMessages() const {
  if (ShouldSkipPermissionWarnings(extension_id_)) {
    return PermissionMessages();
  } else {
    return PermissionMessageProvider::Get()->GetPermissionMessages(
        active_permissions(), manifest_type_);
  }
}
