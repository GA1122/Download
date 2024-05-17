void Browser::ToggleUseVerticalTabs() {
  use_vertical_tabs_.SetValue(!UseVerticalTabs());
  UseVerticalTabsChanged();
}
