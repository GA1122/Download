void ExtensionInstallUI::ConfirmReEnable(Delegate* delegate,
                                         const Extension* extension) {
  DCHECK(ui_loop_ == MessageLoop::current());
  extension_ = extension;
  permissions_ = extension->GetActivePermissions();
  delegate_ = delegate;

  ShowConfirmation(RE_ENABLE_PROMPT);
}
