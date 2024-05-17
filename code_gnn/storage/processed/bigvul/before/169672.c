GURL SiteInstanceImpl::GetSiteForOrigin(const url::Origin& origin) {
  std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(
      origin.host(),
      net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
  std::string site = origin.scheme();
  site += url::kStandardSchemeSeparator;
  site += domain.empty() ? origin.host() : domain;
  return GURL(site);
}
