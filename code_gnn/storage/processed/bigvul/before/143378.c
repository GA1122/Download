std::string Job::MakeDnsCacheKey(const std::string& host,
                                 ResolveDnsOperation op) {
  return base::StringPrintf("%d:%s", op, host.c_str());
}
