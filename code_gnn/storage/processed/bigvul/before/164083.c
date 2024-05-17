bool SortNamespacesByLength(
    const AppCacheNamespace& lhs, const AppCacheNamespace& rhs) {
  return lhs.namespace_url.spec().length() > rhs.namespace_url.spec().length();
}
