bool SyncBackendHost::Core::IsCurrentThreadSafeForModel(
    syncable::ModelType model_type) {
  base::AutoLock lock(host_->registrar_lock_);

  browser_sync::ModelSafeRoutingInfo::const_iterator routing_it =
      host_->registrar_.routing_info.find(model_type);
  if (routing_it == host_->registrar_.routing_info.end())
    return false;
  browser_sync::ModelSafeGroup group = routing_it->second;
  WorkerMap::const_iterator worker_it = host_->registrar_.workers.find(group);
  if (worker_it == host_->registrar_.workers.end())
    return false;
  ModelSafeWorker* worker = worker_it->second;
  return worker->CurrentThreadIsWorkThread();
}
