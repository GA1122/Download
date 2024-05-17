bool IsGoogleHostname(base::StringPiece host,
                      SubdomainPermission subdomain_permission) {
  url::CanonHostInfo host_info;
  return IsCanonicalHostGoogleHostname(net::CanonicalizeHost(host, &host_info),
                                       subdomain_permission);
}
