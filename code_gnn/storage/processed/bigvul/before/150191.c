void StartupBrowserCreator::RegisterLocalStatePrefs(
    PrefRegistrySimple* registry) {
#if !defined(OS_CHROMEOS)
  registry->RegisterBooleanPref(prefs::kPromotionalTabsEnabled, true);
  registry->RegisterBooleanPref(prefs::kCommandLineFlagSecurityWarningsEnabled,
                                true);
#endif
  registry->RegisterBooleanPref(prefs::kSuppressUnsupportedOSWarning, false);
  registry->RegisterBooleanPref(prefs::kWasRestarted, false);
}
