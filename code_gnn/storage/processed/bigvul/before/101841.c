void Browser::TabPinnedStateChanged(TabContentsWrapper* contents, int index) {
  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile());
  if (session_service) {
    session_service->SetPinnedState(
        session_id(),
        GetTabContentsWrapperAt(index)->restore_tab_helper()->session_id(),
        tab_handler_->GetTabStripModel()->IsTabPinned(index));
  }
}
