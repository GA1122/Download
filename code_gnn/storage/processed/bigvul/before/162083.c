RenderProcessHost* RenderProcessHostImpl::FindReusableProcessHostForSite(
    BrowserContext* browser_context,
    const GURL& site_url) {
  if (!ShouldFindReusableProcessHostForSite(browser_context, site_url))
    return nullptr;

  std::set<RenderProcessHost*> eligible_foreground_hosts;
  std::set<RenderProcessHost*> eligible_background_hosts;

  SiteProcessCountTracker* pending_tracker =
      static_cast<SiteProcessCountTracker*>(
          browser_context->GetUserData(kPendingSiteProcessCountTrackerKey));
  if (pending_tracker) {
    pending_tracker->FindRenderProcessesForSite(
        site_url, &eligible_foreground_hosts, &eligible_background_hosts);
  }

  if (eligible_foreground_hosts.empty()) {
    SiteProcessCountTracker* committed_tracker =
        static_cast<SiteProcessCountTracker*>(
            browser_context->GetUserData(kCommittedSiteProcessCountTrackerKey));
    if (committed_tracker) {
      committed_tracker->FindRenderProcessesForSite(
          site_url, &eligible_foreground_hosts, &eligible_background_hosts);
    }
  }

  if (!eligible_foreground_hosts.empty()) {
    int index = base::RandInt(0, eligible_foreground_hosts.size() - 1);
    auto iterator = eligible_foreground_hosts.begin();
    for (int i = 0; i < index; ++i)
      ++iterator;
    return (*iterator);
  }

  if (!eligible_background_hosts.empty()) {
    int index = base::RandInt(0, eligible_background_hosts.size() - 1);
    auto iterator = eligible_background_hosts.begin();
    for (int i = 0; i < index; ++i)
      ++iterator;
    return (*iterator);
  }

  return nullptr;
}
