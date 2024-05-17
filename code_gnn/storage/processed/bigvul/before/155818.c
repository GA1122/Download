void CreateExtensionUpdateRequest(
    const std::string& id,
    PermissionRequestCreator* creator,
    SupervisedUserService::SuccessCallback callback) {
  creator->CreateExtensionUpdateRequest(id, std::move(callback));
}
