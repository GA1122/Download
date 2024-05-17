std::string AppListSyncableService::FindOrCreateOemFolder() {
  AppListFolderItem* oem_folder = model_->FindFolderItem(kOemFolderId);
  if (!oem_folder) {
    scoped_ptr<AppListFolderItem> new_folder(new AppListFolderItem(
        kOemFolderId, AppListFolderItem::FOLDER_TYPE_OEM));
    oem_folder =
        static_cast<AppListFolderItem*>(model_->AddItem(new_folder.Pass()));
    SyncItem* oem_sync_item = FindSyncItem(kOemFolderId);
    if (oem_sync_item) {
      VLOG(1) << "Creating OEM folder from existing sync item: "
               << oem_sync_item->item_ordinal.ToDebugString();
      model_->SetItemPosition(oem_folder, oem_sync_item->item_ordinal);
    } else {
      model_->SetItemPosition(oem_folder, GetOemFolderPos());
    }
  }
  model_->SetItemName(oem_folder, oem_folder_name_);
  return oem_folder->id();
}
