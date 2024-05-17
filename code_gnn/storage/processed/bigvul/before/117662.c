void BrowserInit::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterIntegerPref(
      prefs::kShownAutoLaunchInfobar, 0, PrefService::UNSYNCABLE_PREF);
}
