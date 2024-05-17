base::string16 ExtensionInstallPrompt::Prompt::GetPermissionsDetails(
    size_t index,
    PermissionsType permissions_type) const {
  const InstallPromptPermissions& install_permissions =
      GetPermissionsForType(permissions_type);
  CHECK_LT(index, install_permissions.details.size());
  return install_permissions.details[index];
}
