void PeopleHandler::UpdateSyncStatus() {
  FireWebUIListener("sync-status-changed", *GetSyncStatusDictionary());
}
