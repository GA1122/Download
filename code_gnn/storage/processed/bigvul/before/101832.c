void Browser::SyncHistoryWithTabs(int index) {
  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile());
  if (session_service) {
    for (int i = index; i < tab_count(); ++i) {
      TabContentsWrapper* tab = GetTabContentsWrapperAt(i);
      if (tab) {
        session_service->SetTabIndexInWindow(
            session_id(), tab->restore_tab_helper()->session_id(), i);
        session_service->SetPinnedState(
            session_id(),
            tab->restore_tab_helper()->session_id(),
            tab_handler_->GetTabStripModel()->IsTabPinned(i));
      }
    }
  }
}
