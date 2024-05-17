bool IsGoogleSearchSubdomainUrl(const GURL& url) {
  if (!IsValidURL(url, PortPermission::DISALLOW_NON_STANDARD_PORTS))
    return false;

  base::StringPiece host(url.host_piece());
  StripTrailingDot(&host);

  static const base::NoDestructor<base::flat_set<base::StringPiece>>
      google_subdomains(std::initializer_list<base::StringPiece>(
          {"ipv4.google.com", "ipv6.google.com"}));

  return google_subdomains->contains(host);
}
