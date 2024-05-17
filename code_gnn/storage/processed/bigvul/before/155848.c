void SupervisedUserService::OnSafeSitesSettingChanged() {
  bool use_blacklist = supervised_users::IsSafeSitesBlacklistEnabled(profile_);
  if (use_blacklist != url_filter_.HasBlacklist()) {
    if (use_blacklist && blacklist_state_ == BlacklistLoadState::NOT_LOADED) {
      LoadBlacklist(GetBlacklistPath(), GURL(kBlacklistURL));
    } else if (!use_blacklist ||
               blacklist_state_ == BlacklistLoadState::LOADED) {
      UpdateBlacklist();
    }
  }

  bool use_online_check =
      supervised_users::IsSafeSitesOnlineCheckEnabled(profile_);
  if (use_online_check != url_filter_.HasAsyncURLChecker()) {
    if (use_online_check)
      url_filter_.InitAsyncURLChecker(
          content::BrowserContext::GetDefaultStoragePartition(profile_)
              ->GetURLLoaderFactoryForBrowserProcess());
    else
      url_filter_.ClearAsyncURLChecker();
  }
}
