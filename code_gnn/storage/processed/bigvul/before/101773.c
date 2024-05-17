void Browser::RegisterAppPrefs(const std::string& app_name, Profile* profile) {
  std::string window_pref(prefs::kBrowserWindowPlacement);
  window_pref.append("_");
  window_pref.append(app_name);
  PrefService* prefs = profile->GetPrefs();
  if (!prefs->FindPreference(window_pref.c_str())) {
    prefs->RegisterDictionaryPref(window_pref.c_str(),
                                  PrefService::UNSYNCABLE_PREF);
  }
}
