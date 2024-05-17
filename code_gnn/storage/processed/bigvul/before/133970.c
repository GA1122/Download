size_t AppListSyncableService::GetNumSyncItemsForTest() {
  GetModel();

  return sync_items_.size();
}
