AXObjectCache* Document::ExistingAXObjectCache() const {
  auto& cache_owner = AxObjectCacheOwner();

  if (!cache_owner.GetLayoutView())
    return nullptr;

  return cache_owner.ax_object_cache_.Get();
}
