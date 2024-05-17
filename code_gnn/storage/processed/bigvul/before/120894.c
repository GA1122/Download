bool IsSyncableApp(const Extension* extension) {
  return GetSyncType(extension) == SYNC_TYPE_APP;
}
