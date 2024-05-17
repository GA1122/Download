void SyncBackendHost::Core::StartSavingChanges() {
  save_changes_timer_.Start(FROM_HERE,
      base::TimeDelta::FromSeconds(kSaveChangesIntervalSeconds),
      this, &Core::SaveChanges);
}
