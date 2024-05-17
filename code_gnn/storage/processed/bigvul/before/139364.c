void TestEvictionCallback(int* evict_count,
                          HostCache::Key* key_out,
                          const HostCache::Key& key,
                          const HostCache::Entry& entry) {
  ++*evict_count;
  *key_out = key;
}
