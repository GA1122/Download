HostResolver::RequestInfo Job::MakeDnsRequestInfo(const std::string& host,
                                                  ResolveDnsOperation op) {
  HostPortPair host_port = HostPortPair(host, 80);
  if (op == MY_IP_ADDRESS || op == MY_IP_ADDRESS_EX) {
    host_port.set_host(GetHostName());
  }

  HostResolver::RequestInfo info(host_port);
  if (op == MY_IP_ADDRESS || op == MY_IP_ADDRESS_EX) {
    info.set_is_my_ip_address(true);
  }
  if (op == MY_IP_ADDRESS || op == DNS_RESOLVE) {
    info.set_address_family(ADDRESS_FAMILY_IPV4);
  }

  return info;
}
