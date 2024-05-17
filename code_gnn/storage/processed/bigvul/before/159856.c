void TopSitesCache::SetTopSites(const MostVisitedURLList& top_sites) {
  top_sites_ = top_sites;
  CountForcedURLs();
  GenerateCanonicalURLs();
}
