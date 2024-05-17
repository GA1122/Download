const HostCache::Entry* HostCache::LookupStale(
    const Key& key,
    base::TimeTicks now,
    HostCache::EntryStaleness* stale_out) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  if (caching_is_disabled())
    return nullptr;

  HostCache::Entry* entry = LookupInternal(key);
  if (!entry) {
    RecordLookup(LOOKUP_MISS_ABSENT, now, nullptr);
    return nullptr;
  }

  bool is_stale = entry->IsStale(now, network_changes_);
  entry->CountHit(  is_stale);
  RecordLookup(is_stale ? LOOKUP_HIT_STALE : LOOKUP_HIT_VALID, now, entry);

  if (stale_out)
    entry->GetStaleness(now, network_changes_, stale_out);
  return entry;
}
