size_t SupervisedUserService::FindEnabledPermissionRequestCreator(
    size_t start) {
  for (size_t i = start; i < permissions_creators_.size(); ++i) {
    if (permissions_creators_[i]->IsEnabled())
      return i;
  }
  return permissions_creators_.size();
}
