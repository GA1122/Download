void Browser::MarkHomePageAsChanged(PrefService* pref_service) {
  pref_service->SetBoolean(prefs::kHomePageChanged, true);
  pref_service->ScheduleSavePersistentPrefs();
}
