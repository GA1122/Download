void SessionService::ScheduleReset() {
  set_pending_reset(true);
  STLDeleteElements(&pending_commands());
  tab_to_available_range_.clear();
  windows_tracking_.clear();
  BuildCommandsFromBrowsers(&pending_commands(), &tab_to_available_range_,
                            &windows_tracking_);
  if (!windows_tracking_.empty()) {
    has_open_trackable_browsers_ = true;
    move_on_new_browser_ = true;
  }
  StartSaveTimer();
}
