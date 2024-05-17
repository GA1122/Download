void RenderWidgetHostImpl::CopyFromBackingStore(
    const gfx::Rect& src_subrect,
    const gfx::Size& accelerated_dst_size,
    const base::Callback<void(bool)>& callback,
    skia::PlatformBitmap* output) {
  if (view_ && is_accelerated_compositing_active_) {
    TRACE_EVENT0("browser",
        "RenderWidgetHostImpl::CopyFromBackingStore::FromCompositingSurface");
    gfx::Rect copy_rect = src_subrect.IsEmpty() ?
        gfx::Rect(view_->GetViewBounds().size()) : src_subrect;
    view_->CopyFromCompositingSurface(copy_rect,
                                      accelerated_dst_size,
                                      callback,
                                      output);
    return;
  }

  BackingStore* backing_store = GetBackingStore(false);
  if (!backing_store) {
    callback.Run(false);
    return;
  }

  TRACE_EVENT0("browser",
      "RenderWidgetHostImpl::CopyFromBackingStore::FromBackingStore");
  gfx::Rect copy_rect = src_subrect.IsEmpty() ?
      gfx::Rect(backing_store->size()) : src_subrect;
  bool result = backing_store->CopyFromBackingStore(copy_rect, output);
  callback.Run(result);
}
