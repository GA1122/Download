const GURL& TopSitesCache::GetCanonicalURL(const GURL& url) const {
  CanonicalURLs::const_iterator it = GetCanonicalURLsIterator(url);
  return it == canonical_urls_.end() ? url : it->first.first->url;
}
