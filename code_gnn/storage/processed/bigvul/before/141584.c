bool ShouldSkipSession(const SyncedSession& session) {
  for (const auto& window_pair : session.windows) {
    const sessions::SessionWindow& window = window_pair.second->wrapped_window;
    if (!ShouldSkipWindow(window))
      return false;
  }
  return true;
}
