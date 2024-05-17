void AppListSyncableService::ProcessExistingSyncItem(SyncItem* sync_item) {
  if (sync_item->item_type ==
      sync_pb::AppListSpecifics::TYPE_REMOVE_DEFAULT_APP) {
    return;
  }
  VLOG(2) << "ProcessExistingSyncItem: " << sync_item->ToString();
  AppListItem* app_item = model_->FindItem(sync_item->item_id);
  DVLOG(2) << " AppItem: " << app_item->ToDebugString();
  if (!app_item) {
    LOG(ERROR) << "Item not found in model: " << sync_item->ToString();
    return;
  }
  if (app_list::switches::IsFolderUIEnabled() &&
      app_item->folder_id() != sync_item->parent_id &&
      !AppIsOem(app_item->id())) {
    VLOG(2) << " Moving Item To Folder: " << sync_item->parent_id;
    model_->MoveItemToFolder(app_item, sync_item->parent_id);
  }
  UpdateAppItemFromSyncItem(sync_item, app_item);
}
