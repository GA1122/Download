void SyncBackendHost::GetWorkers(std::vector<ModelSafeWorker*>* out) {
  base::AutoLock lock(registrar_lock_);
  out->clear();
  for (WorkerMap::const_iterator it = registrar_.workers.begin();
       it != registrar_.workers.end(); ++it) {
    out->push_back((*it).second);
  }
}
