void AppCache::InitializeWithManifest(AppCacheManifest* manifest) {
  DCHECK(manifest);
  intercept_namespaces_.swap(manifest->intercept_namespaces);
  fallback_namespaces_.swap(manifest->fallback_namespaces);
  online_whitelist_namespaces_.swap(manifest->online_whitelist_namespaces);
  online_whitelist_all_ = manifest->online_whitelist_all;

  std::sort(intercept_namespaces_.begin(), intercept_namespaces_.end(),
            SortNamespacesByLength);
  std::sort(fallback_namespaces_.begin(), fallback_namespaces_.end(),
            SortNamespacesByLength);
}
