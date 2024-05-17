bool SyncManager::HasUnsyncedItems() const {
  sync_api::ReadTransaction trans(FROM_HERE, GetUserShare());
  return (trans.GetWrappedTrans()->directory()->unsynced_entity_count() != 0);
}
