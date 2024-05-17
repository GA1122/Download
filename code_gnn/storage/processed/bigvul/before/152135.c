ServiceWorkerRegistration* ServiceWorkerContextCore::GetLiveRegistration(
    int64_t id) {
  auto it = live_registrations_.find(id);
  return (it != live_registrations_.end()) ? it->second : nullptr;
}
