void Browser::RegisterPrefs(PrefService* prefs) {
  prefs->RegisterIntegerPref(prefs::kOptionsWindowLastTabIndex, 0);
  prefs->RegisterIntegerPref(prefs::kExtensionSidebarWidth, -1);
  prefs->RegisterBooleanPref(prefs::kAllowFileSelectionDialogs, true);
}
