void Browser::OpenCurrentURL() {
  UserMetrics::RecordAction(UserMetricsAction("LoadURL"), profile_);
  LocationBar* location_bar = window_->GetLocationBar();
  WindowOpenDisposition open_disposition =
      location_bar->GetWindowOpenDisposition();
  if (OpenInstant(open_disposition))
    return;

  GURL url(WideToUTF8(location_bar->GetInputString()));
  browser::NavigateParams params(this, url, location_bar->GetPageTransition());
  params.disposition = open_disposition;
  params.tabstrip_add_types =
      TabStripModel::ADD_FORCE_INDEX | TabStripModel::ADD_INHERIT_OPENER;
  browser::Navigate(&params);

  if (profile_->GetExtensionService()->IsInstalledApp(url)) {
    UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                              extension_misc::APP_LAUNCH_OMNIBOX_LOCATION,
                              extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
  }
}
