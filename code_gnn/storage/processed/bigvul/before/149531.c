void LoadingPredictor::OnNavigationFinished(
    const NavigationID& old_navigation_id,
    const NavigationID& new_navigation_id,
    bool is_error_page) {
  if (shutdown_)
    return;

  loading_data_collector()->RecordFinishNavigation(
      old_navigation_id, new_navigation_id, is_error_page);
  active_navigations_.erase(old_navigation_id);
  CancelPageLoadHint(old_navigation_id.main_frame_url);
}
