void ProfileSyncService::EnableEncryptEverything() {
  DCHECK(sync_initialized());
  if (!encrypt_everything_)
    encryption_pending_ = true;
  UMA_HISTOGRAM_BOOLEAN("Sync.EncryptAllData", true);
}
