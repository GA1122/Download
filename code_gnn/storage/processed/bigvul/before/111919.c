void ProfileSyncService::OnSyncCycleCompleted() {
  UpdateLastSyncedTime();
  if (GetSessionModelAssociator()) {
    MessageLoop::current()->PostTask(FROM_HERE,
        base::Bind(&browser_sync::SessionModelAssociator::DeleteStaleSessions,
                   GetSessionModelAssociator()->AsWeakPtr()));
  }
  DVLOG(2) << "Notifying observers sync cycle completed";
  NotifyObservers();
}
