RenderProcessHost* SiteInstanceImpl::GetProcess() {

  if (!process_) {
    BrowserContext* browser_context = browsing_instance_->browser_context();

    bool should_use_process_per_site =
        has_site_ &&
        RenderProcessHost::ShouldUseProcessPerSite(browser_context, site_);
    if (should_use_process_per_site) {
      process_reuse_policy_ = ProcessReusePolicy::PROCESS_PER_SITE;
    } else if (process_reuse_policy_ == ProcessReusePolicy::PROCESS_PER_SITE) {
      process_reuse_policy_ = ProcessReusePolicy::DEFAULT;
    }

    process_ = RenderProcessHostImpl::GetProcessHostForSiteInstance(
        browser_context, this);

    CHECK(process_);
    process_->AddObserver(this);

    if (process_reuse_policy_ == ProcessReusePolicy::PROCESS_PER_SITE &&
        has_site_) {
      RenderProcessHostImpl::RegisterProcessHostForSite(browser_context,
                                                        process_, site_);
    }

    TRACE_EVENT2("navigation", "SiteInstanceImpl::GetProcess",
                 "site id", id_, "process id", process_->GetID());
    GetContentClient()->browser()->SiteInstanceGotProcess(this);

    if (has_site_)
      LockToOriginIfNeeded();
  }
  DCHECK(process_);

  return process_;
}
