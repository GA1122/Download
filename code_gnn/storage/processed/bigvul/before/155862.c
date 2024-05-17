void SupervisedUserService::UpdateBlacklist() {
  bool use_blacklist = supervised_users::IsSafeSitesBlacklistEnabled(profile_);
  url_filter_.SetBlacklist(use_blacklist ? &blacklist_ : nullptr);
  for (SupervisedUserServiceObserver& observer : observer_list_)
    observer.OnURLFilterChanged();
}
