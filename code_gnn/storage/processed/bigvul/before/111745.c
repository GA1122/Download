void PromoResourceService::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterStringPref(prefs::kNtpPromoResourceCacheUpdate,
                            "0",
                            PrefService::UNSYNCABLE_PREF);
  NotificationPromo::RegisterUserPrefs(prefs);

  prefs->RegisterDoublePref(prefs::kNtpCustomLogoStart,
                            0,
                            PrefService::UNSYNCABLE_PREF);
  prefs->RegisterDoublePref(prefs::kNtpCustomLogoEnd,
                            0,
                            PrefService::UNSYNCABLE_PREF);
  prefs->ClearPref(prefs::kNtpCustomLogoStart);
  prefs->ClearPref(prefs::kNtpCustomLogoEnd);
}
