void Browser::OpenAboutChromeDialog() {
  UserMetrics::RecordAction(UserMetricsAction("AboutChrome"));
#if defined(OS_CHROMEOS)
  std::string chrome_settings(chrome::kChromeUISettingsURL);
  ShowSingletonTab(GURL(chrome_settings.append(chrome::kAboutOptionsSubPage)));
#else
  window_->ShowAboutChromeDialog();
#endif
}
