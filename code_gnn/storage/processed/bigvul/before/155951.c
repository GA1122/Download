void PeopleHandler::OnStateChanged(syncer::SyncService* sync) {
  UpdateSyncStatus();

  PushSyncPrefs();
}
