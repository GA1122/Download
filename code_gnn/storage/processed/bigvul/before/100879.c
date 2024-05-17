void ExtensionInstallUI::ConfirmInstall(Delegate* delegate,
                                        const Extension* extension) {
  DCHECK(ui_loop_ == MessageLoop::current());
  extension_ = extension;
  permissions_ = extension->GetActivePermissions();
  delegate_ = delegate;

  if (extension->is_theme()) {
    delegate->InstallUIProceed();
    return;
  }

  ShowConfirmation(INSTALL_PROMPT);
}
