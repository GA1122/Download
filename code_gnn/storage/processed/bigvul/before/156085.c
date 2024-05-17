bool IsValidHostName(base::StringPiece host,
                     base::StringPiece domain_in_lower_case,
                     SubdomainPermission subdomain_permission,
                     base::StringPiece* tld) {
  if (host.find(domain_in_lower_case) == base::StringPiece::npos)
    return false;

  size_t tld_length =
      net::registry_controlled_domains::GetCanonicalHostRegistryLength(
          host, net::registry_controlled_domains::EXCLUDE_UNKNOWN_REGISTRIES,
          net::registry_controlled_domains::EXCLUDE_PRIVATE_REGISTRIES);
  if ((tld_length == 0) || (tld_length == std::string::npos))
    return false;

  base::StringPiece host_minus_tld =
      host.substr(0, host.length() - tld_length - 1);

  if (tld)
    *tld = host.substr(host.length() - tld_length);

  if (base::LowerCaseEqualsASCII(host_minus_tld, domain_in_lower_case))
    return true;

  if (subdomain_permission == ALLOW_SUBDOMAIN) {
    std::string dot_domain(".");
    domain_in_lower_case.AppendToString(&dot_domain);
    return base::EndsWith(host_minus_tld, dot_domain,
                          base::CompareCase::INSENSITIVE_ASCII);
  }

  std::string www_domain("www.");
  domain_in_lower_case.AppendToString(&www_domain);
  return base::LowerCaseEqualsASCII(host_minus_tld, www_domain);
}
