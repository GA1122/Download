void Browser::ToggleUseCompactNavigationBar() {
  use_compact_navigation_bar_.SetValue(!UseCompactNavigationBar());
  UseCompactNavigationBarChanged();
}
