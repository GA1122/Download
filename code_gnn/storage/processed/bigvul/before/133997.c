void AppListSyncableService::UntrackUninstalledDriveApp(
    const std::string& drive_app_id) {
  const std::string sync_id = GetDriveAppSyncId(drive_app_id);
  SyncItem* sync_item = FindSyncItem(sync_id);
  if (!sync_item)
    return;

  DCHECK_EQ(sync_item->item_type,
            sync_pb::AppListSpecifics::TYPE_REMOVE_DEFAULT_APP);
  DeleteSyncItem(sync_item);
}
