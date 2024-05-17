bool IsSyncableExtension(const Extension* extension) {
  return GetSyncType(extension) == SYNC_TYPE_EXTENSION;
}
