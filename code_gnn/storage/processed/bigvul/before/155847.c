void SupervisedUserService::OnPermissionRequestIssued(
    const CreatePermissionRequestCallback& create_request,
    SuccessCallback callback,
    size_t index,
    bool success) {
  if (success) {
    std::move(callback).Run(true);
    return;
  }

  AddPermissionRequestInternal(create_request, std::move(callback), index + 1);
}
