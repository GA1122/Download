void ProfileSyncService::AcknowledgeSyncedTypes() {
  sync_prefs_.AcknowledgeSyncedTypes(GetRegisteredDataTypes());
}
