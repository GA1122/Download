const GURL& TopSitesCache::GetURLFromIterator(
    CanonicalURLs::const_iterator it) const {
  DCHECK(it != canonical_urls_.end());
  return it->first.first->redirects[it->first.second];
}
