scoped_ptr<net::HostResolver> CreateGlobalHostResolver(net::NetLog* net_log) {
  TRACE_EVENT0("startup", "IOThread::CreateGlobalHostResolver");
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();

  net::HostResolver::Options options;

  if (command_line.HasSwitch(switches::kHostResolverParallelism)) {
    std::string s =
        command_line.GetSwitchValueASCII(switches::kHostResolverParallelism);

    int n;
    if (base::StringToInt(s, &n) && n > 0) {
      options.max_concurrent_resolves = static_cast<size_t>(n);
    } else {
      LOG(ERROR) << "Invalid switch for host resolver parallelism: " << s;
    }
  }

  if (command_line.HasSwitch(switches::kHostResolverRetryAttempts)) {
    std::string s =
        command_line.GetSwitchValueASCII(switches::kHostResolverRetryAttempts);
    int n;
    if (base::StringToInt(s, &n) && n >= 0) {
      options.max_retry_attempts = static_cast<size_t>(n);
    } else {
      LOG(ERROR) << "Invalid switch for host resolver retry attempts: " << s;
    }
  }

  scoped_ptr<net::HostResolver> global_host_resolver(
      net::HostResolver::CreateSystemResolver(options, net_log));

  if (command_line.HasSwitch(switches::kEnableIPv6)) {
    global_host_resolver->SetDefaultAddressFamily(
        net::ADDRESS_FAMILY_UNSPECIFIED);
  } else if (command_line.HasSwitch(switches::kDisableIPv6)) {
    global_host_resolver->SetDefaultAddressFamily(net::ADDRESS_FAMILY_IPV4);
  }

  if (!command_line.HasSwitch(switches::kHostResolverRules))
    return global_host_resolver.PassAs<net::HostResolver>();

  scoped_ptr<net::MappedHostResolver> remapped_resolver(
      new net::MappedHostResolver(global_host_resolver.Pass()));
  remapped_resolver->SetRulesFromString(
      command_line.GetSwitchValueASCII(switches::kHostResolverRules));
  return remapped_resolver.PassAs<net::HostResolver>();
}
