HostCache::Entry::Entry(const HostCache::Entry& entry,
                        base::TimeTicks now,
                        base::TimeDelta ttl,
                        int network_changes)
    : error_(entry.error()),
      addresses_(entry.addresses()),
      ttl_(entry.ttl()),
      expires_(now + ttl),
      network_changes_(network_changes),
      total_hits_(0),
      stale_hits_(0) {}
