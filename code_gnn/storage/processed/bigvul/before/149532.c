void LoadingPredictor::OnNavigationStarted(const NavigationID& navigation_id) {
  if (shutdown_)
    return;

  loading_data_collector()->RecordStartNavigation(navigation_id);
  CleanupAbandonedHintsAndNavigations(navigation_id);
  active_navigations_.emplace(navigation_id);
  PrepareForPageLoad(navigation_id.main_frame_url, HintOrigin::NAVIGATION);
}
