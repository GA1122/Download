void BrowserWindowGtk::RegisterUserPrefs(PrefService* prefs) {
  bool custom_frame_default = false;
  if (ui::XDisplayExists() &&
      !prefs->HasPrefPath(prefs::kUseCustomChromeFrame)) {
    custom_frame_default = GetCustomFramePrefDefault();
  }
  prefs->RegisterBooleanPref(prefs::kUseCustomChromeFrame,
                             custom_frame_default,
                             PrefService::SYNCABLE_PREF);
}
