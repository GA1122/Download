void ChromeGeolocationPermissionContext::RegisterUserPrefs(
    PrefService *user_prefs) {
#if defined(OS_ANDROID)
  user_prefs->RegisterBooleanPref(prefs::kGeolocationEnabled, true,
                                  PrefService::UNSYNCABLE_PREF);
#endif
}
