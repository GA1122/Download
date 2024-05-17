void HostCache::RecordErase(EraseReason reason,
                            base::TimeTicks now,
                            const Entry& entry) {
  HostCache::EntryStaleness stale;
  entry.GetStaleness(now, network_changes_, &stale);
  CACHE_HISTOGRAM_ENUM("Erase", reason, MAX_ERASE_REASON);
  if (stale.is_stale()) {
    CACHE_HISTOGRAM_TIME("EraseStale.ExpiredBy", stale.expired_by);
    CACHE_HISTOGRAM_COUNT("EraseStale.NetworkChanges", stale.network_changes);
    CACHE_HISTOGRAM_COUNT("EraseStale.StaleHits", entry.stale_hits());
  } else {
    CACHE_HISTOGRAM_TIME("EraseValid.ValidFor", -stale.expired_by);
  }
}
