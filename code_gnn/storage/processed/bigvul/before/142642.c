WebstoreStandaloneInstaller::CreateInstallUI() {
  return base::WrapUnique(new ExtensionInstallPrompt(GetWebContents()));
}
