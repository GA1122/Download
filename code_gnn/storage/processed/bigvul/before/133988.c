void AppListSyncableService::ResolveFolderPositions() {
  if (!app_list::switches::IsFolderUIEnabled())
    return;

  VLOG(1) << "ResolveFolderPositions.";
  for (SyncItemMap::iterator iter = sync_items_.begin();
       iter != sync_items_.end(); ++iter) {
    SyncItem* sync_item = iter->second;
    if (sync_item->item_type != sync_pb::AppListSpecifics::TYPE_FOLDER)
      continue;
    AppListItem* app_item = model_->FindItem(sync_item->item_id);
    if (!app_item)
      continue;
    UpdateAppItemFromSyncItem(sync_item, app_item);
  }

  AppListFolderItem* oem_folder = model_->FindFolderItem(kOemFolderId);
  if (oem_folder && !FindSyncItem(kOemFolderId)) {
    model_->SetItemPosition(oem_folder, GetOemFolderPos());
    VLOG(1) << "Creating new OEM folder sync item: "
            << oem_folder->position().ToDebugString();
    CreateSyncItemFromAppItem(oem_folder);
  }
}
