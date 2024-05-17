void WebstoreStandaloneInstaller::OnExtensionInstallFailure(
    const std::string& id,
    const std::string& error,
    WebstoreInstaller::FailureReason reason) {
  CHECK_EQ(id_, id);

  webstore_install::Result install_result = webstore_install::OTHER_ERROR;
  switch (reason) {
    case WebstoreInstaller::FAILURE_REASON_CANCELLED:
      install_result = webstore_install::USER_CANCELLED;
      break;
    case WebstoreInstaller::FAILURE_REASON_DEPENDENCY_NOT_FOUND:
    case WebstoreInstaller::FAILURE_REASON_DEPENDENCY_NOT_SHARED_MODULE:
      install_result = webstore_install::MISSING_DEPENDENCIES;
      break;
    default:
      break;
  }

  CompleteInstall(install_result, error);
}
