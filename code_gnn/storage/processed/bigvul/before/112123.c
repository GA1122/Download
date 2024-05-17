void SyncManager::SyncInternal::StartSyncingNormally() {
  if (scheduler())   
    scheduler()->Start(SyncScheduler::NORMAL_MODE, base::Closure());
}
