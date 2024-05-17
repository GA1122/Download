void TestingAutomationProvider::GetLastNavigationTime(
    int handle,
    int64* last_navigation_time) {
  base::Time time(tab_tracker_->GetLastNavigationTime(handle));
  *last_navigation_time = time.ToInternalValue();
}
