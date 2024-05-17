void HostCache::EvictOneEntry(base::TimeTicks now) {
  DCHECK_LT(0u, entries_.size());

  auto oldest_it = entries_.begin();
  for (auto it = entries_.begin(); it != entries_.end(); ++it) {
    if ((it->second.expires() < oldest_it->second.expires()) &&
        (it->second.IsStale(now, network_changes_) ||
         !oldest_it->second.IsStale(now, network_changes_))) {
      oldest_it = it;
    }
  }

  if (!eviction_callback_.is_null())
    eviction_callback_.Run(oldest_it->first, oldest_it->second);
  RecordErase(ERASE_EVICT, now, oldest_it->second);
  entries_.erase(oldest_it);
}
