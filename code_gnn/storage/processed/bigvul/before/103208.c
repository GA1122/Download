void Browser::TabPinnedStateChanged(TabContentsWrapper* contents, int index) {
  if (!profile()->HasSessionService())
    return;
  SessionService* session_service = profile()->GetSessionService();
  if (session_service) {
    session_service->SetPinnedState(
        session_id(),
        GetTabContentsAt(index)->controller().session_id(),
        tab_handler_->GetTabStripModel()->IsTabPinned(index));
  }
}
