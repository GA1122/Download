void Browser::OpenTabpose() {
#if defined(OS_MACOSX)
  UserMetrics::RecordAction(UserMetricsAction("OpenTabpose"), profile_);
  window()->OpenTabpose();
#else
  NOTREACHED();
#endif
}
