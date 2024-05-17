TopSitesCache::TopSitesCache() : num_forced_urls_(0) {
  clear_query_ref_.ClearQuery();
  clear_query_ref_.ClearRef();
  clear_path_query_ref_.ClearQuery();
  clear_path_query_ref_.ClearRef();
  clear_path_query_ref_.ClearPath();
}
