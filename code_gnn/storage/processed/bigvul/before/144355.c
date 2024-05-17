void ExtensionInstallPrompt::ConfirmPermissionsForDelegatedInstall(
    Delegate* delegate,
    const Extension* extension,
    const std::string& delegated_username,
    const SkBitmap* icon) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  delegate_ = delegate;
  extension_ = extension;
  delegated_username_ = delegated_username;
  SetIcon(icon);
  prompt_ = new Prompt(DELEGATED_PERMISSIONS_PROMPT);
  ShowConfirmation();
}
