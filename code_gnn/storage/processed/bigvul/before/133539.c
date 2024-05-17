bool CollectSites(BrowserContext* context,
                  std::set<GURL>* sites,
                  FrameTreeNode* node) {
  sites->insert(SiteInstance::GetSiteForURL(context, node->current_url()));
  return true;
}
