void WebstoreStandaloneInstaller::OnWebstoreParseFailure(
    const std::string& id,
    InstallHelperResultCode result_code,
    const std::string& error_message) {
  webstore_install::Result install_result = webstore_install::OTHER_ERROR;
  switch (result_code) {
    case WebstoreInstallHelper::Delegate::MANIFEST_ERROR:
      install_result = webstore_install::INVALID_MANIFEST;
      break;
    case WebstoreInstallHelper::Delegate::ICON_ERROR:
      install_result = webstore_install::ICON_ERROR;
      break;
    default:
      break;
  }

  CompleteInstall(install_result, error_message);
}
