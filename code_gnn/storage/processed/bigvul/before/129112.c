std::vector<base::string16> PermissionsData::GetPermissionMessageStrings()
    const {
  if (ShouldSkipPermissionWarnings(extension_id_))
    return std::vector<base::string16>();
  return PermissionMessageProvider::Get()->GetWarningMessages(
      active_permissions(), manifest_type_);
}
