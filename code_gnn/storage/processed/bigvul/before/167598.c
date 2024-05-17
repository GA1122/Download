bool SiteInstanceImpl::HasProcess() const {
  if (process_ != nullptr)
    return true;

  BrowserContext* browser_context =
      browsing_instance_->browser_context();
  if (has_site_ &&
      RenderProcessHost::ShouldUseProcessPerSite(browser_context, site_) &&
      RenderProcessHostImpl::GetProcessHostForSite(browser_context, site_)) {
    return true;
  }

  return false;
}
