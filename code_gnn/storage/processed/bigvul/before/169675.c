bool SiteInstanceImpl::HasWrongProcessForURL(const GURL& url) {
  if (!HasProcess())
    return false;

  if (IsRendererDebugURL(url))
    return false;

  if (url.IsAboutBlank() && site_ != GURL(kUnreachableWebDataURL))
    return false;

  GURL site_url =
      SiteInstance::GetSiteForURL(browsing_instance_->browser_context(), url);
  GURL origin_lock =
      DetermineProcessLockURL(browsing_instance_->browser_context(), url);
  return !RenderProcessHostImpl::IsSuitableHost(
      GetProcess(), browsing_instance_->browser_context(), site_url,
      origin_lock);
}
