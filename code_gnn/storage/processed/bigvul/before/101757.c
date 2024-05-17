void Browser::OpenTabpose() {
#if defined(OS_MACOSX)
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kEnableExposeForTabs)) {
    return;
  }

  UserMetrics::RecordAction(UserMetricsAction("OpenTabpose"));
  window()->OpenTabpose();
#else
  NOTREACHED();
#endif
}
