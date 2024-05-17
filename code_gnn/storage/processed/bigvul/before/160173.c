std::unique_ptr<MemBackendImpl> MemBackendImpl::CreateBackend(
    int max_bytes,
    net::NetLog* net_log) {
  std::unique_ptr<MemBackendImpl> cache(
      std::make_unique<MemBackendImpl>(net_log));
  cache->SetMaxSize(max_bytes);
  if (cache->Init())
    return cache;

  LOG(ERROR) << "Unable to create cache";
  return nullptr;
}
