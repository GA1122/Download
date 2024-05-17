void AppListSyncableService::AddItem(scoped_ptr<AppListItem> app_item) {
  SyncItem* sync_item = FindOrAddSyncItem(app_item.get());
  if (!sync_item)
    return;   

  std::string folder_id;
  if (app_list::switches::IsFolderUIEnabled()) {
    if (AppIsOem(app_item->id())) {
      folder_id = FindOrCreateOemFolder();
      VLOG_IF(2, !folder_id.empty())
          << this << ": AddItem to OEM folder: " << sync_item->ToString();
    } else {
      folder_id = sync_item->parent_id;
    }
  }
  VLOG(2) << this << ": AddItem: " << sync_item->ToString()
          << " Folder: '" << folder_id << "'";
  model_->AddItemToFolder(app_item.Pass(), folder_id);
}
