void Browser::RegisterPrefs(PrefService* prefs) {
  prefs->RegisterIntegerPref(prefs::kOptionsWindowLastTabIndex, 0);
  prefs->RegisterIntegerPref(prefs::kExtensionSidebarWidth, -1);
  prefs->RegisterIntegerPref(prefs::kMultipleProfilePrefMigration, 0);
  prefs->RegisterBooleanPref(prefs::kAllowFileSelectionDialogs, true);
#if defined(GOOGLE_CHROME_BUILD)
  prefs->RegisterBooleanPref(prefs::kClearPluginLSODataEnabled, true);
#else
  prefs->RegisterBooleanPref(prefs::kClearPluginLSODataEnabled, false);
#endif
}
