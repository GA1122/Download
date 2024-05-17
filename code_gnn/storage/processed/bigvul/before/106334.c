bool SyncBackendHost::IsNigoriEnabled() const {
  base::AutoLock lock(registrar_lock_);
  return registrar_.routing_info.find(syncable::NIGORI) !=
      registrar_.routing_info.end();
}
