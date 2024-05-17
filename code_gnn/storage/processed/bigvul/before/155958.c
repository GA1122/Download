void PeopleHandler::SyncStartupCompleted() {
  ProfileSyncService* service = GetSyncService();
  DCHECK(service->IsEngineInitialized());

  engine_start_timer_.reset();

  sync_startup_tracker_.reset();

  PushSyncPrefs();
}
