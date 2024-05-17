void Job::SaveDnsToLocalCache(const std::string& host,
                              ResolveDnsOperation op,
                              int net_error,
                              const AddressList& addresses) {
  CheckIsOnOriginThread();

  std::string cache_value;
  if (net_error != OK) {
    cache_value = std::string();
  } else if (op == DNS_RESOLVE || op == MY_IP_ADDRESS) {
    cache_value = addresses.front().ToStringWithoutPort();
  } else {
    for (AddressList::const_iterator iter = addresses.begin();
         iter != addresses.end(); ++iter) {
      if (!cache_value.empty())
        cache_value += ";";
      cache_value += iter->ToStringWithoutPort();
    }
  }

  dns_cache_[MakeDnsCacheKey(host, op)] = cache_value;
}
