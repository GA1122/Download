void Browser::SyncHistoryWithTabs(int index) {
  if (!profile()->HasSessionService())
    return;
  SessionService* session_service = profile()->GetSessionService();
  if (session_service) {
    for (int i = index; i < tab_count(); ++i) {
      TabContents* contents = GetTabContentsAt(i);
      if (contents) {
        session_service->SetTabIndexInWindow(
            session_id(), contents->controller().session_id(), i);
        session_service->SetPinnedState(
            session_id(),
            contents->controller().session_id(),
            tab_handler_->GetTabStripModel()->IsTabPinned(i));
      }
    }
  }
}
