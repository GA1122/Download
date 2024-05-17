NTPResourceCache::NTPResourceCache(Profile* profile) : profile_(profile) {
  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
                 content::Source<ThemeService>(
                     ThemeServiceFactory::GetForProfile(profile)));
  registrar_.Add(this, chrome::NOTIFICATION_PROMO_RESOURCE_STATE_CHANGED,
                 content::NotificationService::AllSources());
  registrar_.Add(this, chrome::NTP4_INTRO_PREF_CHANGED,
                 content::NotificationService::AllSources());

  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(prefs::kSyncAcknowledgedSyncTypes, this);
  pref_change_registrar_.Add(prefs::kShowBookmarkBar, this);
  pref_change_registrar_.Add(prefs::kNTPShownPage, this);
  pref_change_registrar_.Add(prefs::kSyncPromoShowNTPBubble, this);
}
