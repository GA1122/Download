bool ProfileSyncService::IsSyncEnabled() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(switches::kDisableSync);
}
