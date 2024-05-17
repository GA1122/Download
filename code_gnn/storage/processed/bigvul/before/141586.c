bool ShouldSkipWindow(const sessions::SessionWindow& window) {
  for (const auto& tab_ptr : window.tabs) {
    const sessions::SessionTab& session_tab = *(tab_ptr.get());
    if (!ShouldSkipTab(session_tab))
      return false;
  }
  return true;
}
