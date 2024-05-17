PermissionsData::GetPermissionMessageDetailsStrings() const {
  if (ShouldSkipPermissionWarnings(extension_id_))
    return std::vector<base::string16>();
  return PermissionMessageProvider::Get()->GetWarningMessagesDetails(
      active_permissions(), manifest_type_);
}
