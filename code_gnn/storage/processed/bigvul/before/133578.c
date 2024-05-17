std::set<GURL> WebContentsImpl::GetSitesInTab() const {
  std::set<GURL> sites;
  frame_tree_.ForEach(base::Bind(&CollectSites,
                                 base::Unretained(GetBrowserContext()),
                                 base::Unretained(&sites)));
  return sites;
}
