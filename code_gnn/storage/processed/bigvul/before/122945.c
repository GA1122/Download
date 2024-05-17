bool RenderWidgetHostImpl::CopyFromBackingStoreToCGContext(
    const CGRect& dest_rect, CGContextRef target) {
  BackingStore* backing_store = GetBackingStore(false);
  if (!backing_store)
    return false;
  (static_cast<BackingStoreMac*>(backing_store))->
      CopyFromBackingStoreToCGContext(dest_rect, target);
  return true;
}
