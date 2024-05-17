ExtensionInstallPrompt::Prompt::GetPermissionsForType(
    PermissionsType permissions_type) {
  DCHECK_NE(ALL_PERMISSIONS, permissions_type);
  return permissions_type == REGULAR_PERMISSIONS ? prompt_permissions_
                                                 : withheld_prompt_permissions_;
}
