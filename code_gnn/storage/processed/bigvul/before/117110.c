void SessionService::WindowClosing(const SessionID& window_id) {
  if (!ShouldTrackChangesToWindow(window_id))
    return;

  if (has_open_trackable_browsers_) {
    has_open_trackable_browsers_ = HasOpenTrackableBrowsers(window_id);
  }
  if (should_record_close_as_pending())
    pending_window_close_ids_.insert(window_id.id());
  else
    window_closing_ids_.insert(window_id.id());
}
