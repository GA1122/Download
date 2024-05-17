void SyncManager::SyncInternal::StartSyncingNormally() {
  if (scheduler())   
    scheduler()->Start(SyncScheduler::NORMAL_MODE, NULL);
}
