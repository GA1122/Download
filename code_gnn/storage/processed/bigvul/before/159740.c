AXObjectCache* Document::AxObjectCache() const {
  Settings* settings = GetSettings();
  if (!settings || !settings->GetAccessibilityEnabled())
    return nullptr;

  Document& cache_owner = AxObjectCacheOwner();

  if (!cache_owner.GetLayoutView())
    return nullptr;

  DCHECK(&cache_owner == this || !ax_object_cache_);
  if (!cache_owner.ax_object_cache_)
    cache_owner.ax_object_cache_ = AXObjectCache::Create(cache_owner);
  return cache_owner.ax_object_cache_.Get();
}
