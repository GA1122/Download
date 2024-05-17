void SupervisedUserService::AddExtensionInstallRequest(
    const std::string& extension_id,
    const base::Version& version) {
  std::string id = GetExtensionRequestId(extension_id, version);
  AddExtensionInstallRequest(extension_id, version,
                             base::BindOnce(ExtensionInstallRequestSent, id));
}
