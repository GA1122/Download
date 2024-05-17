void Browser::UseVerticalTabsChanged() {
  UpdateTabStripModelInsertionPolicy();
  window()->ToggleTabStripMode();
}
