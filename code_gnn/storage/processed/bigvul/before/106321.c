void SyncBackendHost::GetModelSafeRoutingInfo(ModelSafeRoutingInfo* out) {
  base::AutoLock lock(registrar_lock_);
  ModelSafeRoutingInfo copy(registrar_.routing_info);
  out->swap(copy);
}
