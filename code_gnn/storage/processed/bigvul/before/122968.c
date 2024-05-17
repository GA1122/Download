gfx::Size RenderWidgetHostImpl::GetBackingStoreSize() {
  BackingStore* backing_store = GetBackingStore(false);
  return backing_store ? backing_store->size() : gfx::Size();
}
