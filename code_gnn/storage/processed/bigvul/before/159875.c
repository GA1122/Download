bool TopSitesImpl::GetPageThumbnail(
    const GURL& url,
    bool prefix_match,
    scoped_refptr<base::RefCountedMemory>* bytes) {
  {
    base::AutoLock lock(lock_);
    if (thread_safe_cache_->GetPageThumbnail(url, bytes))
      return true;
  }

  for (const auto& prepopulated_page : prepopulated_pages_) {
    if (url == prepopulated_page.most_visited.url) {
      *bytes =
          ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytesForScale(
              prepopulated_page.thumbnail_id, ui::SCALE_FACTOR_100P);
      return true;
    }
  }

  if (prefix_match) {
    std::vector<GURL> url_list;
    url_list.push_back(url);
    if (url.SchemeIsHTTPOrHTTPS())
      url_list.push_back(ToggleHTTPAndHTTPS(url));

    for (const GURL& url : url_list) {
      base::AutoLock lock(lock_);

      GURL canonical_url = thread_safe_cache_->GetGeneralizedCanonicalURL(url);
      if (!canonical_url.is_empty() &&
          thread_safe_cache_->GetPageThumbnail(canonical_url, bytes)) {
        return true;
      }
    }
  }

  return false;
}
