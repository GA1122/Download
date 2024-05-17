syncer::StringOrdinal AppListSyncableService::GetOemFolderPos() {
  VLOG(1) << "GetOemFolderPos: " << first_app_list_sync_;
  if (!first_app_list_sync_) {
    VLOG(1) << "Sync items exist, placing OEM folder at end.";
    syncer::StringOrdinal last;
    for (SyncItemMap::iterator iter = sync_items_.begin();
         iter != sync_items_.end(); ++iter) {
      SyncItem* sync_item = iter->second;
      if (sync_item->item_ordinal.IsValid() &&
          (!last.IsValid() || sync_item->item_ordinal.GreaterThan(last))) {
        last = sync_item->item_ordinal;
      }
    }
    if (last.IsValid())
      return last.CreateAfter();
  }

  AppListItemList* item_list = model_->top_level_item_list();
  if (item_list->item_count() == 0)
    return syncer::StringOrdinal();

  size_t oem_index = 0;
  for (; oem_index < item_list->item_count() - 1; ++oem_index) {
    AppListItem* cur_item = item_list->item_at(oem_index);
    if (cur_item->id() == extensions::kWebStoreAppId)
      break;
  }
  syncer::StringOrdinal oem_ordinal;
  AppListItem* prev = item_list->item_at(oem_index);
  if (oem_index + 1 < item_list->item_count()) {
    AppListItem* next = item_list->item_at(oem_index + 1);
    oem_ordinal = prev->position().CreateBetween(next->position());
  } else {
    oem_ordinal = prev->position().CreateAfter();
  }
  VLOG(1) << "Placing OEM Folder at: " << oem_index
          << " position: " << oem_ordinal.ToDebugString();
  return oem_ordinal;
}
