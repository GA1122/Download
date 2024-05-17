RenderFrameHostManager::SiteInstanceDescriptor::SiteInstanceDescriptor(
    BrowserContext* browser_context,
    GURL dest_url,
    SiteInstanceRelation relation_to_current)
    : existing_site_instance(nullptr), relation(relation_to_current) {
  new_site_url = SiteInstance::GetSiteForURL(browser_context, dest_url);
}
