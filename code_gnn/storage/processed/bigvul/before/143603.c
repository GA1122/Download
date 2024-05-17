void RenderWidgetHostImpl::CopyFromBackingStore(
    const gfx::Rect& src_subrect,
    const gfx::Size& accelerated_dst_size,
    const ReadbackRequestCallback& callback,
    const SkColorType preferred_color_type) {
  if (view_) {
    TRACE_EVENT0("browser",
        "RenderWidgetHostImpl::CopyFromBackingStore::FromCompositingSurface");
    gfx::Rect accelerated_copy_rect = src_subrect.IsEmpty() ?
        gfx::Rect(view_->GetViewBounds().size()) : src_subrect;
    view_->CopyFromCompositingSurface(accelerated_copy_rect,
                                      accelerated_dst_size, callback,
                                      preferred_color_type);
    return;
  }

  callback.Run(SkBitmap(), content::READBACK_FAILED);
}
