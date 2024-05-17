void Browser::Exit() {
  UserMetrics::RecordAction(UserMetricsAction("Exit"), profile_);
#if defined(OS_CHROMEOS)
  chromeos::BootTimesLoader::Get()->AddLogoutTimeMarker("LogoutStarted", false);
  const char kLogoutStarted[] = "logout-started";
  chromeos::BootTimesLoader::Get()->RecordCurrentStats(kLogoutStarted);

  PrefService* state = g_browser_process->local_state();
  if (state) {
    std::string owner_locale = state->GetString(prefs::kOwnerLocale);
    if (!owner_locale.empty() &&
        state->GetString(prefs::kApplicationLocale) != owner_locale &&
        !state->IsManagedPreference(prefs::kApplicationLocale)) {
      state->SetString(prefs::kApplicationLocale, owner_locale);
      state->ScheduleSavePersistentPrefs();
    }
  }
#endif
  BrowserList::Exit();
}
