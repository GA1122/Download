void AppListSyncableService::UpdateAppItemFromSyncItem(
    const AppListSyncableService::SyncItem* sync_item,
    AppListItem* app_item) {
  VLOG(2) << this << " UpdateAppItemFromSyncItem: " << sync_item->ToString();
  if (!app_item->position().Equals(sync_item->item_ordinal))
    model_->SetItemPosition(app_item, sync_item->item_ordinal);
  if (sync_item->item_name != app_item->name() &&
      sync_item->item_id != kOemFolderId &&
      (app_item->GetItemType() == AppListFolderItem::kItemType ||
       app_item->name().empty())) {
    model_->SetItemName(app_item, sync_item->item_name);
  }
}
