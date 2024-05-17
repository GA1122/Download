void AdjustNavigateParamsForURL(browser::NavigateParams* params) {
  if (!params->target_contents &&
      browser::IsURLAllowedInIncognito(params->url)) {
    Profile* profile =
        params->browser ? params->browser->profile() : params->profile;

    if ((profile->IsOffTheRecord() && !Profile::IsGuestSession()) ||
        params->disposition == OFF_THE_RECORD) {
      profile = profile->GetOriginalProfile();

      params->disposition = SINGLETON_TAB;
      params->profile = profile;
      params->browser = Browser::GetOrCreateTabbedBrowser(profile);
      params->window_action = browser::NavigateParams::SHOW_WINDOW;
    }
  }
}
