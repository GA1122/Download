void SupervisedUserService::AddPermissionRequestCreator(
    std::unique_ptr<PermissionRequestCreator> creator) {
  permissions_creators_.push_back(std::move(creator));
}
