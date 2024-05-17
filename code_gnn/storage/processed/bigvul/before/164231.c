  void AddHosts(const std::set<AppCacheHost*>& hosts) {
    for (AppCacheHost* host : hosts)
      AddHost(host);
  }
