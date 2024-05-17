void Browser::OpenOptionsDialog() {
  UserMetrics::RecordAction(UserMetricsAction("ShowOptions"), profile_);
  GURL url(chrome::kChromeUISettingsURL);
  browser::NavigateParams params(GetSingletonTabNavigateParams(url));
  params.path_behavior = browser::NavigateParams::IGNORE_AND_STAY_PUT;
  browser::Navigate(&params);
}
