void HTMLCanvasElement::DoDeferredPaintInvalidation() {
  DCHECK(!dirty_rect_.IsEmpty());
  if (LowLatencyEnabled()) {
    return;
  }
  LayoutBox* layout_box = GetLayoutBox();
  if (Is2d()) {
    FloatRect src_rect(0, 0, Size().Width(), Size().Height());
    dirty_rect_.Intersect(src_rect);

    FloatRect invalidation_rect;
    if (layout_box) {
      FloatRect content_rect(layout_box->PhysicalContentBoxRect());
      FloatRect mapped_dirty_rect =
          MapRect(dirty_rect_, src_rect, content_rect);
      if (context_->IsComposited()) {
        mapped_dirty_rect.MoveBy(-content_rect.Location());
      }
      invalidation_rect = mapped_dirty_rect;
    } else {
      invalidation_rect = dirty_rect_;
    }

    if (dirty_rect_.IsEmpty())
      return;

    if (canvas2d_bridge_)
      canvas2d_bridge_->DoPaintInvalidation(invalidation_rect);
  }

  if (context_ && HasImageBitmapContext() && context_->CcLayer())
    context_->CcLayer()->SetNeedsDisplay();

  NotifyListenersCanvasChanged();
  did_notify_listeners_for_current_frame_ = true;

  if (layout_box && (!context_ || !context_->IsComposited())) {
    FloatRect src_rect(0, 0, Size().Width(), Size().Height());
    FloatRect content_rect(layout_box->PhysicalContentBoxRect());
    if (content_rect.Width() > src_rect.Width() ||
        content_rect.Height() > src_rect.Height()) {
      dirty_rect_.Inflate(0.5);
    }

    dirty_rect_.Intersect(src_rect);
    LayoutRect mapped_dirty_rect(
        EnclosingIntRect(MapRect(dirty_rect_, src_rect, content_rect)));
    DisableCompositingQueryAsserts disabler;
    layout_box->InvalidatePaintRectangle(mapped_dirty_rect);
  }
  dirty_rect_ = FloatRect();

  DCHECK(dirty_rect_.IsEmpty());
}
