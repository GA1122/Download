  PrerenderManager* prerender_manager() const {
    Profile* profile = browser()->GetSelectedTabContents()->profile();
    PrerenderManager* prerender_manager = profile->GetPrerenderManager();
    return prerender_manager;
  }
