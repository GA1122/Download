base::Optional<url::Origin> GetOriginForURLLoaderFactory(
    GURL target_url,
    SiteInstanceImpl* site_instance) {
  if (target_url.SchemeIs(url::kAboutScheme)) {
    if (SiteIsolationPolicy::UseDedicatedProcessesForAllSites())
      return url::Origin::Create(site_instance->GetSiteURL());
    return base::nullopt;
  }

  return url::Origin::Create(target_url);
}
