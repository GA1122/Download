bool RenderProcessHost::ShouldTryToUseExistingProcessHost(
    BrowserContext* browser_context,
    const GURL& url) {
  if (run_renderer_in_process())
    return true;

  if (g_all_hosts.Get().size() >= GetMaxRendererProcessCount())
    return true;

  return GetContentClient()->browser()->ShouldTryToUseExistingProcessHost(
      browser_context, url);
}
