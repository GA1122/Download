void ExtensionInstallPrompt::ConfirmPermissions(
    Delegate* delegate,
    const Extension* extension,
    scoped_ptr<const PermissionSet> permissions) {
  DCHECK(ui_loop_ == base::MessageLoop::current());
  extension_ = extension;
  custom_permissions_ = permissions.Pass();
  delegate_ = delegate;
  prompt_ = new Prompt(PERMISSIONS_PROMPT);

  LoadImageIfNeeded();
}
