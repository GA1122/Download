bool ExtensionService::GetSyncData(
    const Extension& extension,
    ExtensionFilter filter,
    ExtensionSyncData* extension_sync_data) const {
  if (!(*filter)(extension)) {
    return false;
  }
  *extension_sync_data = GetSyncDataHelper(extension);
  return true;
}
