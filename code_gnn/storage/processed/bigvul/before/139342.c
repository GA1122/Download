HostCache::Entry::Entry(int error,
                        const AddressList& addresses,
                        base::TimeDelta ttl)
    : error_(error), addresses_(addresses), ttl_(ttl) {
  DCHECK(ttl >= base::TimeDelta());
}
