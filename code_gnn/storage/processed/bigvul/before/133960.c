void AppListSyncableService::DeleteSyncItem(SyncItem* sync_item) {
  if (SyncStarted()) {
    VLOG(2) << this << " -> SYNC DELETE: " << sync_item->ToString();
    SyncChange sync_change(FROM_HERE, SyncChange::ACTION_DELETE,
                           GetSyncDataFromSyncItem(sync_item));
    sync_processor_->ProcessSyncChanges(
        FROM_HERE, syncer::SyncChangeList(1, sync_change));
  }
  std::string item_id = sync_item->item_id;
  delete sync_item;
  sync_items_.erase(item_id);
}
