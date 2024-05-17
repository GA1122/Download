void Browser::Exit() {
  UserMetrics::RecordAction(UserMetricsAction("Exit"), profile_);
#if defined(OS_CHROMEOS)
  chromeos::BootTimesLoader::Get()->AddLogoutTimeMarker("LogoutStarted", false);
  const char kLogoutStarted[] = "logout-started";
  chromeos::BootTimesLoader::Get()->RecordCurrentStats(kLogoutStarted);
#endif
  BrowserList::Exit();
}
