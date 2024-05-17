void RenderProcessHostImpl::AddExpectedNavigationToSite(
    BrowserContext* browser_context,
    RenderProcessHost* render_process_host,
    const GURL& site_url) {
  if (!ShouldTrackProcessForSite(browser_context, render_process_host,
                                 site_url))
    return;

  SiteProcessCountTracker* tracker = static_cast<SiteProcessCountTracker*>(
      browser_context->GetUserData(kPendingSiteProcessCountTrackerKey));
  if (!tracker) {
    tracker = new SiteProcessCountTracker();
    browser_context->SetUserData(kPendingSiteProcessCountTrackerKey,
                                 base::WrapUnique(tracker));
  }
  tracker->IncrementSiteProcessCount(site_url, render_process_host->GetID());
}
