void SupervisedUserService::OnCustodianInfoChanged() {
  for (SupervisedUserServiceObserver& observer : observer_list_)
    observer.OnCustodianInfoChanged();
}
