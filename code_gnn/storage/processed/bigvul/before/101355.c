bool SessionModelAssociator::SyncLocalWindowToSyncModel(
    const SessionWindow& window) {
  DCHECK(CalledOnValidThread());
  DCHECK(tab_map_.empty());
  for (size_t i = 0; i < window.tabs.size(); ++i) {
    SessionTab* tab = window.tabs[i];
    int64 id = tab_pool_.GetFreeTabNode();
    if (id == sync_api::kInvalidId) {
      LOG(ERROR) << "Failed to find/generate free sync node for tab.";
      return false;
    }

    sync_api::WriteTransaction trans(FROM_HERE, sync_service_->GetUserShare());
    if (!WriteSessionTabToSyncModel(*tab, id, &trans)) {
      return false;
    }

    TabLinks t(id, tab);
    tab_map_[tab->tab_id.id()] = t;
  }
  return true;
}
