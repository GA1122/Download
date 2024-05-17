void HostCache::RecordUpdateStale(AddressListDeltaType delta,
                                  const EntryStaleness& stale) {
  CACHE_HISTOGRAM_ENUM("UpdateStale.AddressListDelta", delta, MAX_DELTA_TYPE);
  switch (delta) {
    case DELTA_IDENTICAL:
      CACHE_HISTOGRAM_TIME("UpdateStale.ExpiredBy_Identical", stale.expired_by);
      CACHE_HISTOGRAM_COUNT("UpdateStale.NetworkChanges_Identical",
                            stale.network_changes);
      break;
    case DELTA_REORDERED:
      CACHE_HISTOGRAM_TIME("UpdateStale.ExpiredBy_Reordered", stale.expired_by);
      CACHE_HISTOGRAM_COUNT("UpdateStale.NetworkChanges_Reordered",
                            stale.network_changes);
      break;
    case DELTA_OVERLAP:
      CACHE_HISTOGRAM_TIME("UpdateStale.ExpiredBy_Overlap", stale.expired_by);
      CACHE_HISTOGRAM_COUNT("UpdateStale.NetworkChanges_Overlap",
                            stale.network_changes);
      break;
    case DELTA_DISJOINT:
      CACHE_HISTOGRAM_TIME("UpdateStale.ExpiredBy_Disjoint", stale.expired_by);
      CACHE_HISTOGRAM_COUNT("UpdateStale.NetworkChanges_Disjoint",
                            stale.network_changes);
      break;
    case MAX_DELTA_TYPE:
      NOTREACHED();
      break;
  }
}
