bool ExtensionInstallPrompt::Prompt::ShouldShowPermissions() const {
  return GetPermissionCount(ALL_PERMISSIONS) > 0 ||
          type_ == POST_INSTALL_PERMISSIONS_PROMPT;
 }
