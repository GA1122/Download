void RenderProcessHostImpl::RemoveFrameWithSite(
    BrowserContext* browser_context,
    RenderProcessHost* render_process_host,
    const GURL& site_url) {
  if (!ShouldTrackProcessForSite(browser_context, render_process_host,
                                 site_url))
    return;

  SiteProcessCountTracker* tracker = static_cast<SiteProcessCountTracker*>(
      browser_context->GetUserData(kCommittedSiteProcessCountTrackerKey));
  if (!tracker) {
    tracker = new SiteProcessCountTracker();
    browser_context->SetUserData(kCommittedSiteProcessCountTrackerKey,
                                 base::WrapUnique(tracker));
  }
  tracker->DecrementSiteProcessCount(site_url, render_process_host->GetID());
}
