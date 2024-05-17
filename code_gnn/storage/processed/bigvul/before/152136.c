ServiceWorkerVersion* ServiceWorkerContextCore::GetLiveVersion(int64_t id) {
  auto it = live_versions_.find(id);
  return (it != live_versions_.end()) ? it->second : nullptr;
}
