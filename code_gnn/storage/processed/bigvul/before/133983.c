void AppListSyncableService::PruneEmptySyncFolders() {
  if (!app_list::switches::IsFolderUIEnabled())
    return;

  std::set<std::string> parent_ids;
  for (SyncItemMap::iterator iter = sync_items_.begin();
       iter != sync_items_.end(); ++iter) {
    parent_ids.insert(iter->second->parent_id);
  }
  for (SyncItemMap::iterator iter = sync_items_.begin();
       iter != sync_items_.end(); ) {
    SyncItem* sync_item = (iter++)->second;
    if (sync_item->item_type != sync_pb::AppListSpecifics::TYPE_FOLDER)
      continue;
    if (!ContainsKey(parent_ids, sync_item->item_id))
      DeleteSyncItem(sync_item);
  }
}
