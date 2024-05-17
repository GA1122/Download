bool ChromeContentBrowserClient::MayReuseHost(
    content::RenderProcessHost* process_host) {
  prerender::PrerenderManager* prerender_manager =
      prerender::PrerenderManagerFactory::GetForBrowserContext(
          process_host->GetBrowserContext());
  if (prerender_manager &&
      !prerender_manager->MayReuseProcessHost(process_host)) {
    return false;
  }

  return true;
}
