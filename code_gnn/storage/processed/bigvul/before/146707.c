AXObjectCache* Document::AxObjectCache() const {
  Settings* settings = this->GetSettings();
  if (!settings || !settings->GetAccessibilityEnabled())
    return 0;

  Document& cache_owner = this->AxObjectCacheOwner();

  if (!cache_owner.GetLayoutView())
    return 0;

  DCHECK(&cache_owner == this || !ax_object_cache_);
  if (!cache_owner.ax_object_cache_)
    cache_owner.ax_object_cache_ = AXObjectCache::Create(cache_owner);
  return cache_owner.ax_object_cache_.Get();
}
