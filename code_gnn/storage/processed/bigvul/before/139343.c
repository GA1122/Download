HostCache::Entry::Entry(int error, const AddressList& addresses)
    : error_(error),
      addresses_(addresses),
      ttl_(base::TimeDelta::FromSeconds(-1)) {}
