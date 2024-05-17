std::unique_ptr<HostCache> HostCache::CreateDefaultCache() {
#if defined(ENABLE_BUILT_IN_DNS)
  const size_t kDefaultMaxEntries = 1000;
#else
  const size_t kDefaultMaxEntries = 100;
#endif
  const size_t kSaneMaxEntries = 1 << 20;
  size_t max_entries = 0;
  base::StringToSizeT(base::FieldTrialList::FindFullName("HostCacheSize"),
                      &max_entries);
  if ((max_entries == 0) || (max_entries > kSaneMaxEntries))
    max_entries = kDefaultMaxEntries;
  return base::WrapUnique(new HostCache(max_entries));
}
