GURL TopSitesCache::GetGeneralizedCanonicalURL(const GURL& url) const {
  CanonicalURLs::const_iterator it_hi =
      canonical_urls_.lower_bound(CanonicalURLQuery(url).entry());
  if (it_hi != canonical_urls_.end()) {
    if (url.ReplaceComponents(clear_query_ref_) ==
        GetURLFromIterator(it_hi).ReplaceComponents(clear_query_ref_)) {
      return it_hi->first.first->url;
    }
  }

  GURL base_url(url.ReplaceComponents(clear_path_query_ref_));
  CanonicalURLs::const_iterator it_lo =
      canonical_urls_.lower_bound(CanonicalURLQuery(base_url).entry());
  if (it_lo == canonical_urls_.end())
    return GURL::EmptyGURL();
  GURL compare_url_lo(GetURLFromIterator(it_lo));
  if (!HaveSameSchemeHostAndPort(base_url, compare_url_lo) ||
      !IsPathPrefix(base_url.path(), compare_url_lo.path())) {
    return GURL::EmptyGURL();
  }

  for (CanonicalURLs::const_iterator it = it_hi; it != it_lo;) {
    --it;
    GURL compare_url(GetURLFromIterator(it));
    DCHECK(HaveSameSchemeHostAndPort(compare_url, url));
    if (IsPathPrefix(compare_url.path(), url.path()))
      return it->first.first->url;
  }

  return GURL::EmptyGURL();
}
