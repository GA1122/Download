void ExtensionInstallUI::ConfirmPermissions(
    Delegate* delegate,
    const Extension* extension,
    const ExtensionPermissionSet* permissions) {
  DCHECK(ui_loop_ == MessageLoop::current());
  extension_ = extension;
  permissions_ = permissions;
  delegate_ = delegate;

  ShowConfirmation(PERMISSIONS_PROMPT);
}
