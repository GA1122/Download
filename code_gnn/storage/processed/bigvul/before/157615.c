  explicit OneTimeCachingHostResolver(const HostPortPair& host_port)
      : MockHostResolverBase(  true), host_port_(host_port) {}
