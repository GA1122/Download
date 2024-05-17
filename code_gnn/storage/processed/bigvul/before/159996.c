bool DiskCacheBackendTest::CreateSetOfRandomEntries(
    std::set<std::string>* key_pool) {
  const int kNumEntries = 10;
  const int initial_entry_count = cache_->GetEntryCount();

  for (int i = 0; i < kNumEntries; ++i) {
    std::string key = GenerateKey(true);
    disk_cache::Entry* entry;
    if (CreateEntry(key, &entry) != net::OK) {
      return false;
    }
    key_pool->insert(key);
    entry->Close();
  }
  return key_pool->size() ==
         static_cast<size_t>(cache_->GetEntryCount() - initial_entry_count);
}
