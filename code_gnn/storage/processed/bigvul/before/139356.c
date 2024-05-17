void HostCache::RecordLookup(LookupOutcome outcome,
                             base::TimeTicks now,
                             const Entry* entry) {
  CACHE_HISTOGRAM_ENUM("Lookup", outcome, MAX_LOOKUP_OUTCOME);
  switch (outcome) {
    case LOOKUP_MISS_ABSENT:
    case LOOKUP_MISS_STALE:
    case LOOKUP_HIT_VALID:
      break;
    case LOOKUP_HIT_STALE:
      CACHE_HISTOGRAM_TIME("LookupStale.ExpiredBy", now - entry->expires());
      CACHE_HISTOGRAM_COUNT("LookupStale.NetworkChanges",
                            network_changes_ - entry->network_changes());
      break;
    case MAX_LOOKUP_OUTCOME:
      NOTREACHED();
      break;
  }
}
