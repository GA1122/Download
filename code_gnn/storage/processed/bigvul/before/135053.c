AppCacheBackendImpl::~AppCacheBackendImpl() {
  STLDeleteValues(&hosts_);
  if (service_)
    service_->UnregisterBackend(this);
}
