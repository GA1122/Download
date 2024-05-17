void BackendImpl::OnExternalCacheHit(const std::string& key) {
  background_queue_.OnExternalCacheHit(key);
}
