void SiteInstanceImpl::SetSite(const GURL& url) {
  TRACE_EVENT2("navigation", "SiteInstanceImpl::SetSite",
               "site id", id_, "url", url.possibly_invalid_spec());
  DCHECK(!has_site_);

  has_site_ = true;
  BrowserContext* browser_context = browsing_instance_->browser_context();
  site_ = GetSiteForURL(browser_context, url);
  original_url_ = url;

  browsing_instance_->RegisterSiteInstance(this);

  bool should_use_process_per_site =
      RenderProcessHost::ShouldUseProcessPerSite(browser_context, site_);
  if (should_use_process_per_site) {
    process_reuse_policy_ = ProcessReusePolicy::PROCESS_PER_SITE;
  }

  if (process_) {
    LockToOriginIfNeeded();

    if (should_use_process_per_site) {
      RenderProcessHostImpl::RegisterProcessHostForSite(
          browser_context, process_, site_);
    }
  }
}
