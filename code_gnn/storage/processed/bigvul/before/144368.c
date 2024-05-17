base::string16 ExtensionInstallPrompt::Prompt::GetPermission(
    size_t index,
    PermissionsType permissions_type) const {
  const InstallPromptPermissions& install_permissions =
      GetPermissionsForType(permissions_type);
  CHECK_LT(index, install_permissions.permissions.size());
  return install_permissions.permissions[index];
}
