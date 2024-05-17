void SessionService::SetWindowType(const SessionID& window_id,
                                   Browser::Type type) {
  if (!should_track_changes_for_browser_type(type))
    return;

  windows_tracking_.insert(window_id.id());

  CommitPendingCloses();

  has_open_trackable_browsers_ = true;
  move_on_new_browser_ = true;

  ScheduleCommand(
      CreateSetWindowTypeCommand(window_id, WindowTypeForBrowserType(type)));
}
