void SessionModelAssociator::Associate(const SyncedTabDelegate* tab,
                                       int64 sync_id) {
  DCHECK(CalledOnValidThread());
  SessionID::id_type session_id = tab->GetSessionId();
  const SyncedWindowDelegate* window =
      SyncedWindowDelegate::FindSyncedWindowDelegateWithId(
          tab->GetWindowId());
  DCHECK(window);

  TabLinks t(sync_id, tab);
  tab_map_[session_id] = t;

  sync_api::WriteTransaction trans(FROM_HERE, sync_service_->GetUserShare());
  WriteTabContentsToSyncModel(*window, *tab, sync_id, &trans);
}
