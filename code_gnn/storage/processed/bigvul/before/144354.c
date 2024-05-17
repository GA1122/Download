void ExtensionInstallPrompt::ConfirmPermissionsForDelegatedBundleInstall(
    extensions::BundleInstaller* bundle,
    const std::string& delegated_username,
    const SkBitmap* icon,
    scoped_ptr<const extensions::PermissionSet> permissions) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  bundle_ = bundle;
  delegated_username_ = delegated_username;
  custom_permissions_ = permissions.Pass();
  delegate_ = bundle;
  prompt_ = new Prompt(DELEGATED_BUNDLE_PERMISSIONS_PROMPT);

  SetIcon(icon);
  ShowConfirmation();
}
