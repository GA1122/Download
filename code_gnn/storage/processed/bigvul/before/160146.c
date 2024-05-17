void BackendIO::OnExternalCacheHit(const std::string& key) {
  operation_ = OP_ON_EXTERNAL_CACHE_HIT;
  key_ = key;
}
