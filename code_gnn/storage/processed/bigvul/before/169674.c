GURL SiteInstanceImpl::GetSiteForURL(BrowserContext* browser_context,
                                     const GURL& real_url,
                                     bool should_use_effective_urls) {
  if (real_url.SchemeIs(kGuestScheme))
    return real_url;

  GURL url = should_use_effective_urls
                 ? SiteInstanceImpl::GetEffectiveURL(browser_context, real_url)
                 : real_url;
  url::Origin origin = url::Origin::Create(url);

  if (!origin.host().empty() && origin.scheme() != url::kFileScheme) {
    GURL site_url(GetSiteForOrigin(origin));

    auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
    url::Origin isolated_origin;
    if (policy->GetMatchingIsolatedOrigin(origin, site_url, &isolated_origin))
      return isolated_origin.GetURL();

    if (should_use_effective_urls && url != real_url) {
      std::string non_translated_site_url(
          GetSiteForURL(browser_context, real_url,
                        false  )
              .spec());
      GURL::Replacements replacements;
      replacements.SetRefStr(non_translated_site_url.c_str());
      site_url = site_url.ReplaceComponents(replacements);
    }

    return site_url;
  }

  if (!origin.opaque()) {
    DCHECK(!origin.scheme().empty());
    return GURL(origin.scheme() + ":");
  } else if (url.has_scheme()) {
    bool is_site_isolation_enabled =
        SiteIsolationPolicy::UseDedicatedProcessesForAllSites() ||
        SiteIsolationPolicy::AreIsolatedOriginsEnabled();
    if (is_site_isolation_enabled &&
        (url.SchemeIsBlob() || url.scheme() == url::kDataScheme)) {
      if (url.has_ref()) {
        GURL::Replacements replacements;
        replacements.ClearRef();
        url = url.ReplaceComponents(replacements);
      }
      return url;
    }

    DCHECK(!url.scheme().empty());
    return GURL(url.scheme() + ":");
  }

  DCHECK(!url.is_valid()) << url;
  return GURL();
}
