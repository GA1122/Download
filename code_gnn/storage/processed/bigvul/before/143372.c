bool Job::GetDnsFromLocalCache(const std::string& host,
                               ResolveDnsOperation op,
                               std::string* output,
                               bool* return_value) {
  CheckIsOnWorkerThread();

  DnsCache::const_iterator it = dns_cache_.find(MakeDnsCacheKey(host, op));
  if (it == dns_cache_.end())
    return false;

  *output = it->second;
  *return_value = !it->second.empty();
  return true;
}
