const HostCache::Entry* HostCache::Lookup(const Key& key,
                                          base::TimeTicks now) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  if (caching_is_disabled())
    return nullptr;

  HostCache::Entry* entry = LookupInternal(key);
  if (!entry) {
    RecordLookup(LOOKUP_MISS_ABSENT, now, nullptr);
    return nullptr;
  }
  if (entry->IsStale(now, network_changes_)) {
    RecordLookup(LOOKUP_MISS_STALE, now, entry);
    return nullptr;
  }

  entry->CountHit(  false);
  RecordLookup(LOOKUP_HIT_VALID, now, entry);
  return entry;
}
