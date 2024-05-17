AXObjectCache* Document::ExistingAXObjectCache() const {
  if (!AxObjectCacheOwner().GetLayoutView())
    return 0;

  return AxObjectCacheOwner().ax_object_cache_.Get();
}
