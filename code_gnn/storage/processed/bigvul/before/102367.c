void ExtensionHelper::OnInlineWebstoreInstallResponse(
    int install_id,
    bool success,
    const std::string& error) {
  ChromeWebstoreExtension::HandleInstallResponse(install_id, success, error);
}
