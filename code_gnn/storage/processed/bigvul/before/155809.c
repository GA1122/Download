void SupervisedUserService::AddExtensionInstallRequest(
    const std::string& extension_id,
    const base::Version& version,
    SuccessCallback callback) {
  std::string id = GetExtensionRequestId(extension_id, version);
  AddPermissionRequestInternal(
      base::BindRepeating(CreateExtensionInstallRequest, id),
      std::move(callback), 0);
}
