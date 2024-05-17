bool AppCache::FindResponseForRequest(const GURL& url,
    AppCacheEntry* found_entry, GURL* found_intercept_namespace,
    AppCacheEntry* found_fallback_entry, GURL* found_fallback_namespace,
    bool* found_network_namespace) {
  GURL url_no_ref;
  if (url.has_ref()) {
    GURL::Replacements replacements;
    replacements.ClearRef();
    url_no_ref = url.ReplaceComponents(replacements);
  } else {
    url_no_ref = url;
  }


  AppCacheEntry* entry = GetEntry(url_no_ref);
  if (entry) {
    *found_entry = *entry;
    return true;
  }

  *found_network_namespace = IsInNetworkNamespace(url_no_ref);
  if (*found_network_namespace)
    return true;

  const AppCacheNamespace* intercept_namespace =
      FindInterceptNamespace(url_no_ref);
  if (intercept_namespace) {
    entry = GetEntry(intercept_namespace->target_url);
    DCHECK(entry);
    *found_entry = *entry;
    *found_intercept_namespace = intercept_namespace->namespace_url;
    return true;
  }

  const AppCacheNamespace* fallback_namespace =
      FindFallbackNamespace(url_no_ref);
  if (fallback_namespace) {
    entry = GetEntry(fallback_namespace->target_url);
    DCHECK(entry);
    *found_fallback_entry = *entry;
    *found_fallback_namespace = fallback_namespace->namespace_url;
    return true;
  }

  *found_network_namespace = online_whitelist_all_;
  return *found_network_namespace;
}
