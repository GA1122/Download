bool RenderWidgetHostImpl::CopyFromBackingStoreToGtkWindow(
    const gfx::Rect& dest_rect, GdkWindow* target) {
  BackingStore* backing_store = GetBackingStore(false);
  if (!backing_store)
    return false;
  (static_cast<BackingStoreGtk*>(backing_store))->PaintToRect(
      dest_rect, target);
  return true;
}
