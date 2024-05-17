void ProfileSyncService::StopAndSuppress() {
  sync_prefs_.SetStartSuppressed(true);
  ShutdownImpl(false);
}
