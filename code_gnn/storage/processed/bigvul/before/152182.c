ServiceWorkerContextCore::~ServiceWorkerContextCore() {
  DCHECK(storage_);
  for (const auto& it : live_versions_)
    it.second->RemoveObserver(this);
  weak_factory_.InvalidateWeakPtrs();
}
