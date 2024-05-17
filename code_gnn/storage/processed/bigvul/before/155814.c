void SupervisedUserService::AddPermissionRequestInternal(
    const CreatePermissionRequestCallback& create_request,
    SuccessCallback callback,
    size_t index) {
  size_t next_index = FindEnabledPermissionRequestCreator(index);
  if (next_index >= permissions_creators_.size()) {
    std::move(callback).Run(false);
    return;
  }

  create_request.Run(
      permissions_creators_[next_index].get(),
      base::BindOnce(&SupervisedUserService::OnPermissionRequestIssued,
                     weak_ptr_factory_.GetWeakPtr(), create_request,
                     std::move(callback), next_index));
}
