 virtual bool DnsResolve(const std::string& host, std::string* ip_address) {
    dns_resolves.push_back(host);
 *ip_address = dns_resolve_result;
 return !dns_resolve_result.empty();
 }
