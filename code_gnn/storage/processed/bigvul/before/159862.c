void TopSitesCacheTest::InitTopSiteCache(const char** spec, size_t size) {
  BuildTopSites(spec, size);
  cache_.SetTopSites(top_sites_);
}
