void ExtensionInstallPrompt::Prompt::SetPermissions(
    const PermissionMessages& permissions,
    PermissionsType permissions_type) {
  InstallPromptPermissions& install_permissions =
      GetPermissionsForType(permissions_type);

  install_permissions.permissions.clear();
  install_permissions.details.clear();
  install_permissions.is_showing_details.clear();

  for (const PermissionMessage& msg : permissions) {
    install_permissions.permissions.push_back(msg.message());
    base::string16 details;
    if (!msg.submessages().empty()) {
      std::vector<base::string16> detail_lines_with_bullets;
      for (const auto& detail_line : msg.submessages()) {
        detail_lines_with_bullets.push_back(base::ASCIIToUTF16("- ") +
                                            detail_line);
      }

      details = base::JoinString(detail_lines_with_bullets,
                                 base::ASCIIToUTF16("\n"));
    }
    install_permissions.details.push_back(details);
    install_permissions.is_showing_details.push_back(false);
  }
}
