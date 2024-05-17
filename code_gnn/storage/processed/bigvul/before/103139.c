void Browser::OpenAboutChromeDialog() {
  UserMetrics::RecordAction(UserMetricsAction("AboutChrome"), profile_);
#if defined(OS_CHROMEOS)
  ShowSingletonTab(GURL(chrome::kChromeUIAboutURL));
#else
  window_->ShowAboutChromeDialog();
#endif
}
