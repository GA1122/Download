void SessionModelAssociator::ReassociateTab(const SyncedTabDelegate& tab) {
  DCHECK(CalledOnValidThread());
  int64 sync_id;
  SessionID::id_type id = tab.GetSessionId();
  if (tab.IsBeingDestroyed()) {
    TabLinksMap::iterator tab_iter = tab_map_.find(id);
    if (tab_iter == tab_map_.end()) {
      return;
    }
    tab_pool_.FreeTabNode(tab_iter->second.sync_id());
    tab_map_.erase(tab_iter);
    return;
  }

  if (!IsValidTab(tab))
    return;

  TabLinksMap::const_iterator tablink = tab_map_.find(id);
  if (tablink == tab_map_.end()) {
    sync_id = tab_pool_.GetFreeTabNode();
    if (sync_id == sync_api::kInvalidId)
      return;
  } else {
    sync_id = tablink->second.sync_id();
  }
  Associate(&tab, sync_id);
}
