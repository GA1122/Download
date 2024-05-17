void TopSitesCache::StoreRedirectChain(const RedirectList& redirects,
                                       size_t destination) {

  for (size_t i = 0; i < redirects.size(); i++) {
    if (!IsKnownURL(redirects[i])) {
      CanonicalURLEntry entry;
      entry.first = &(top_sites_[destination]);
      entry.second = i;
      canonical_urls_[entry] = destination;
    }
  }
}
