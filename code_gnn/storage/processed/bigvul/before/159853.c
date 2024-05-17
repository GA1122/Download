size_t TopSitesCache::GetURLIndex(const GURL& url) const {
  DCHECK(IsKnownURL(url));
  return GetCanonicalURLsIterator(url)->second;
}
