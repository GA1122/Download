 bool IsCanonicalHostGoogleHostname(base::StringPiece canonical_host,
                                   SubdomainPermission subdomain_permission) {
  const GURL& base_url(CommandLineGoogleBaseURL());
  if (base_url.is_valid() && (canonical_host == base_url.host_piece()))
    return true;

  base::StringPiece tld;
  if (!IsValidHostName(canonical_host, "google", subdomain_permission, &tld))
    return false;

  StripTrailingDot(&tld);

  static const base::NoDestructor<base::flat_set<base::StringPiece>>
      google_tlds(std::initializer_list<base::StringPiece>({GOOGLE_TLD_LIST}));
  return google_tlds->contains(tld);
}
