bool Job::ResolveDnsBlocking(const std::string& host,
                             ResolveDnsOperation op,
                             std::string* output) {
  CheckIsOnWorkerThread();

  bool rv;
  if (GetDnsFromLocalCache(host, op, output, &rv)) {
    return rv;
  }

  if (dns_cache_.size() >= kMaxUniqueResolveDnsPerExec) {
    return false;
  }

  if (!PostDnsOperationAndWait(host, op, NULL))
    return false;   

  CHECK(GetDnsFromLocalCache(host, op, output, &rv));
  return rv;
}
