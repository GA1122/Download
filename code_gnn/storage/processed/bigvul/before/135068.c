const AppCacheHost* AppCacheHost::GetSpawningHost() const {
  AppCacheBackendImpl* backend = service_->GetBackend(spawning_process_id_);
  return backend ? backend->GetHost(spawning_host_id_) : NULL;
}
