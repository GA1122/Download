void ExtensionInstallPrompt::ConfirmBundleInstall(
    extensions::BundleInstaller* bundle,
    const SkBitmap* icon,
    scoped_ptr<const PermissionSet> permissions) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  bundle_ = bundle;
  custom_permissions_ = permissions.Pass();
  delegate_ = bundle;
  prompt_ = new Prompt(BUNDLE_INSTALL_PROMPT);

  SetIcon(icon);
  ShowConfirmation();
}
