ExtensionInstallPrompt::GetViewsShowDialogCallback() {
  return base::Bind(&ShowExtensionInstallDialogImpl);
}
