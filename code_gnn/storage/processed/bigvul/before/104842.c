  bool UrlIsInPrerenderManager(const GURL& url) {
    return (prerender_manager()->FindEntry(url) != NULL);
  }
