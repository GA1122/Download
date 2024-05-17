void CreateExtensionInstallRequest(
    const std::string& id,
    PermissionRequestCreator* creator,
    SupervisedUserService::SuccessCallback callback) {
  creator->CreateExtensionInstallRequest(id, std::move(callback));
}
