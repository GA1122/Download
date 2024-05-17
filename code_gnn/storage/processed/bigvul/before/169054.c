bool GetMatchingURLCountAndMostRecentCreationTime(
    const std::map<int64_t, OfflinePageItem>& offline_pages,
    std::string name_space,
    const GURL& url,
    base::Time created_before,
    int* matching_url_count,
    base::TimeDelta* most_recent_creation_time) {
  int count = 0;

  base::Time latest_time;
  bool matching_page = false;

  for (auto& id_page_pair : offline_pages) {
    if (id_page_pair.second.client_id.name_space == name_space &&
        url == id_page_pair.second.url) {
      count++;
      base::Time page_creation_time = id_page_pair.second.creation_time;
      if (page_creation_time < created_before &&
          page_creation_time > latest_time) {
        latest_time = page_creation_time;
        matching_page = true;
      }
    }
  }

  if (matching_url_count != nullptr)
    *matching_url_count = count;
  if (most_recent_creation_time != nullptr && latest_time != base::Time())
    *most_recent_creation_time = created_before - latest_time;

  return matching_page;
}
