void SupervisedUserService::OnSiteListUpdated() {
  for (SupervisedUserServiceObserver& observer : observer_list_)
    observer.OnURLFilterChanged();
}
