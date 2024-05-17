RenderFrameHostManager::SiteInstanceDescriptor::SiteInstanceDescriptor(
    BrowserContext* browser_context,
    GURL dest_url,
    SiteInstanceRelation relation_to_current)
    : existing_site_instance(nullptr),
      dest_url(dest_url),
      browser_context(browser_context),
      relation(relation_to_current) {}
