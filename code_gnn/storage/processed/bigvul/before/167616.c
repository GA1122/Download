bool SiteInstanceImpl::ShouldAssignSiteForURL(const GURL& url) {
  if (url == url::kAboutBlankURL)
    return false;

  return GetContentClient()->browser()->ShouldAssignSiteForURL(url);
}
