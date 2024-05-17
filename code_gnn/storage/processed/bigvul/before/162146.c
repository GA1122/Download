bool ShouldTrackProcessForSite(BrowserContext* browser_context,
                               RenderProcessHost* render_process_host,
                               const GURL& site_url) {
  if (site_url.is_empty())
    return false;

  return ShouldUseSiteProcessTracking(
      browser_context, render_process_host->GetStoragePartition(), site_url);
}
