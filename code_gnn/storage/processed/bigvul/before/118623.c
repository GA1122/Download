bool NTPResourceCache::NewTabCacheNeedsRefresh() {
#if defined(OS_MACOSX)
  bool is_enabled = platform_util::IsSwipeTrackingFromScrollEventsEnabled();
  if (is_enabled != is_swipe_tracking_from_scroll_events_enabled_) {
    is_swipe_tracking_from_scroll_events_enabled_ = is_enabled;
    return true;
  }
#endif
  bool should_show_apps_page = NewTabUI::ShouldShowApps();
  if (should_show_apps_page != should_show_apps_page_) {
    should_show_apps_page_ = should_show_apps_page;
    return true;
  }
  return false;
}
