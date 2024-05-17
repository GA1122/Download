std::string GetDriveAppIdFromSyncId(const std::string& sync_id) {
  if (!IsDriveAppSyncId(sync_id))
    return std::string();
  return sync_id.substr(strlen(kDriveAppSyncIdPrefix));
}
