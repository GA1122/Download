void SupervisedUserService::SetActive(bool active) {
  if (active_ == active)
    return;
  active_ = active;

  if (!delegate_ || !delegate_->SetActive(active_)) {
    if (active_) {
#if !defined(OS_ANDROID)
      ProfileOAuth2TokenService* token_service =
          ProfileOAuth2TokenServiceFactory::GetForProfile(profile_);
      token_service->LoadCredentials(
          supervised_users::kSupervisedUserPseudoEmail);
#else
      NOTREACHED();
#endif
    }
  }


#if !defined(OS_ANDROID)
  ThemeService* theme_service = ThemeServiceFactory::GetForProfile(profile_);
  if (theme_service->UsingDefaultTheme() || theme_service->UsingSystemTheme())
    theme_service->UseDefaultTheme();
#endif

  browser_sync::ProfileSyncService* sync_service =
      ProfileSyncServiceFactory::GetForProfile(profile_);
  sync_service->SetEncryptEverythingAllowed(!active_);

  GetSettingsService()->SetActive(active_);

#if BUILDFLAG(ENABLE_EXTENSIONS)
  SetExtensionsActive();
#endif

  if (active_) {
    pref_change_registrar_.Add(
        prefs::kDefaultSupervisedUserFilteringBehavior,
        base::BindRepeating(
            &SupervisedUserService::OnDefaultFilteringBehaviorChanged,
            base::Unretained(this)));
#if BUILDFLAG(ENABLE_EXTENSIONS)
    pref_change_registrar_.Add(
        prefs::kSupervisedUserApprovedExtensions,
        base::BindRepeating(&SupervisedUserService::UpdateApprovedExtensions,
                            base::Unretained(this)));
#endif
    pref_change_registrar_.Add(
        prefs::kSupervisedUserSafeSites,
        base::BindRepeating(&SupervisedUserService::OnSafeSitesSettingChanged,
                            base::Unretained(this)));
    pref_change_registrar_.Add(
        prefs::kSupervisedUserManualHosts,
        base::BindRepeating(&SupervisedUserService::UpdateManualHosts,
                            base::Unretained(this)));
    pref_change_registrar_.Add(
        prefs::kSupervisedUserManualURLs,
        base::BindRepeating(&SupervisedUserService::UpdateManualURLs,
                            base::Unretained(this)));
    for (const char* pref : kCustodianInfoPrefs) {
      pref_change_registrar_.Add(
          pref,
          base::BindRepeating(&SupervisedUserService::OnCustodianInfoChanged,
                              base::Unretained(this)));
    }

    OnDefaultFilteringBehaviorChanged();
    OnSafeSitesSettingChanged();
    whitelist_service_->Init();
    UpdateManualHosts();
    UpdateManualURLs();

#if BUILDFLAG(ENABLE_EXTENSIONS)
    UpdateApprovedExtensions();
#endif

#if !defined(OS_ANDROID)
    BrowserList::AddObserver(this);
#endif
  } else {
    permissions_creators_.clear();
    url_reporter_.reset();

    pref_change_registrar_.Remove(
        prefs::kDefaultSupervisedUserFilteringBehavior);
#if BUILDFLAG(ENABLE_EXTENSIONS)
    pref_change_registrar_.Remove(prefs::kSupervisedUserApprovedExtensions);
#endif
    pref_change_registrar_.Remove(prefs::kSupervisedUserManualHosts);
    pref_change_registrar_.Remove(prefs::kSupervisedUserManualURLs);
    for (const char* pref : kCustodianInfoPrefs) {
      pref_change_registrar_.Remove(pref);
    }

    url_filter_.Clear();
    for (SupervisedUserServiceObserver& observer : observer_list_)
      observer.OnURLFilterChanged();

#if !defined(OS_ANDROID)
    BrowserList::RemoveObserver(this);
#endif
  }
}
