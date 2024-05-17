bool SiteInstanceImpl::HasWrongProcessForURL(const GURL& url) {
  if (!HasProcess())
    return false;

  if (IsRendererDebugURL(url))
    return false;

  if (url == url::kAboutBlankURL)
    return false;

  GURL site_url = GetSiteForURL(browsing_instance_->browser_context(), url);
  return !RenderProcessHostImpl::IsSuitableHost(
      GetProcess(), browsing_instance_->browser_context(), site_url);
}
