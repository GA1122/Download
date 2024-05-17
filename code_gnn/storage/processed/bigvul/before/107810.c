void Browser::RegisterPrefs(PrefService* prefs) {
  prefs->RegisterDictionaryPref(prefs::kBrowserWindowPlacement);
  prefs->RegisterIntegerPref(prefs::kOptionsWindowLastTabIndex, 0);
  prefs->RegisterIntegerPref(prefs::kDevToolsSplitLocation, -1);
  prefs->RegisterDictionaryPref(prefs::kPreferencesWindowPlacement);
  prefs->RegisterIntegerPref(prefs::kExtensionSidebarWidth, -1);
#if defined(GOOGLE_CHROME_BUILD)
  prefs->RegisterBooleanPref(prefs::kClearPluginLSODataEnabled, true);
#else
  prefs->RegisterBooleanPref(prefs::kClearPluginLSODataEnabled, false);
#endif
}
