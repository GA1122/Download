AppCacheHost* AppCacheHost::GetParentAppCacheHost() const {
  DCHECK(is_for_dedicated_worker());
  AppCacheBackendImpl* backend = service_->GetBackend(parent_process_id_);
  return backend ? backend->GetHost(parent_host_id_) : NULL;
}
