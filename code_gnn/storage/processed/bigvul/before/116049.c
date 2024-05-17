void NTPResourceCache::Observe(int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (chrome::NOTIFICATION_BROWSER_THEME_CHANGED == type ||
      chrome::NOTIFICATION_PROMO_RESOURCE_STATE_CHANGED == type) {
    new_tab_incognito_html_ = NULL;
    new_tab_html_ = NULL;
    new_tab_incognito_css_ = NULL;
    new_tab_css_ = NULL;
  } else if (chrome::NOTIFICATION_PREF_CHANGED == type ||
              chrome::NTP4_INTRO_PREF_CHANGED) {
    new_tab_incognito_html_ = NULL;
    new_tab_html_ = NULL;
  } else {
    NOTREACHED();
  }
}
