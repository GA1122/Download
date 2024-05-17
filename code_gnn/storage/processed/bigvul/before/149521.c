void LoadingDataCollector::RecordStartNavigation(
    const NavigationID& navigation_id) {
  CleanupAbandonedNavigations(navigation_id);

  inflight_navigations_.emplace(
      navigation_id,
      std::make_unique<PageRequestSummary>(navigation_id.main_frame_url));
}
