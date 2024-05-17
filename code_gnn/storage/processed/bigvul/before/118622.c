NTPResourceCache::NTPResourceCache(Profile* profile)
    : profile_(profile), is_swipe_tracking_from_scroll_events_enabled_(false),
      should_show_apps_page_(NewTabUI::ShouldShowApps()),
      should_show_most_visited_page_(true),
      should_show_other_devices_menu_(true),
      should_show_recently_closed_menu_(true) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
                 content::Source<ThemeService>(
                     ThemeServiceFactory::GetForProfile(profile)));
  registrar_.Add(this, chrome::NOTIFICATION_PROMO_RESOURCE_STATE_CHANGED,
                 content::NotificationService::AllSources());

  base::Closure callback = base::Bind(&NTPResourceCache::OnPreferenceChanged,
                                      base::Unretained(this));

  profile_pref_change_registrar_.Init(profile_->GetPrefs());
  profile_pref_change_registrar_.Add(prefs::kShowBookmarkBar, callback);
  profile_pref_change_registrar_.Add(prefs::kNtpShownPage, callback);
  profile_pref_change_registrar_.Add(prefs::kSignInPromoShowNTPBubble,
                                     callback);
  profile_pref_change_registrar_.Add(prefs::kHideWebStoreIcon, callback);

#if defined(ENABLE_APP_LIST)
  if (g_browser_process->local_state()) {
    local_state_pref_change_registrar_.Init(g_browser_process->local_state());
    local_state_pref_change_registrar_.Add(prefs::kShowAppLauncherPromo,
                                           callback);
    local_state_pref_change_registrar_.Add(
        prefs::kAppLauncherHasBeenEnabled, callback);
  }
#endif
}
