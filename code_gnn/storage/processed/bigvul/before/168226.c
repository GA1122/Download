bool SwapInPrerender(const GURL& url,
                     prerender::PrerenderManager::Params* params) {
  Profile* profile = Profile::FromBrowserContext(
      params->contents_being_navigated->GetBrowserContext());
  prerender::PrerenderManager* prerender_manager =
      prerender::PrerenderManagerFactory::GetForBrowserContext(profile);
  return prerender_manager &&
      prerender_manager->MaybeUsePrerenderedPage(url, params);
}
