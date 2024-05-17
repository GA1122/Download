void FinalCleanupCallback(disk_cache::BackendImpl* backend) {
  backend->CleanupCache();
}
