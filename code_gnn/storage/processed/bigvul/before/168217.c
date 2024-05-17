bool AdjustNavigateParamsForURL(NavigateParams* params) {
  if (params->contents_to_insert || params->switch_to_singleton_tab ||
      IsURLAllowedInIncognito(params->url, params->initiating_profile) ||
      params->initiating_profile->IsGuestSession()) {
    return true;
  }

  Profile* profile = params->initiating_profile;

  if (profile->IsOffTheRecord() ||
      params->disposition == WindowOpenDisposition::OFF_THE_RECORD) {
    profile = profile->GetOriginalProfile();

    PrefService* prefs = profile->GetPrefs();
    if (prefs && IncognitoModePrefs::GetAvailability(prefs) ==
            IncognitoModePrefs::FORCED) {
      return false;
    }

    params->disposition = WindowOpenDisposition::SINGLETON_TAB;
    params->browser = GetOrCreateBrowser(profile, params->user_gesture);
    params->window_action = NavigateParams::SHOW_WINDOW;
  }

  return true;
}
