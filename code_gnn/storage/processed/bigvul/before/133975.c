bool IsDriveAppSyncId(const std::string& sync_id) {
  return StartsWithASCII(sync_id, kDriveAppSyncIdPrefix, true);
}
