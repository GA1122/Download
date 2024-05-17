void HTMLCanvasElement::Reset() {
  if (ignore_reset_)
    return;

  dirty_rect_ = FloatRect();

  bool had_resource_provider = HasResourceProvider();

  unsigned w = 0;
  AtomicString value = getAttribute(kWidthAttr);
  if (value.IsEmpty() || !ParseHTMLNonNegativeInteger(value, w) ||
      w > 0x7fffffffu) {
    w = kDefaultCanvasWidth;
  }

  unsigned h = 0;
  value = getAttribute(kHeightAttr);
  if (value.IsEmpty() || !ParseHTMLNonNegativeInteger(value, h) ||
      h > 0x7fffffffu) {
    h = kDefaultCanvasHeight;
  }

  if (Is2d()) {
    context_->Reset();
    origin_clean_ = true;
  }

  IntSize old_size = Size();
  IntSize new_size(w, h);

  if (had_resource_provider && old_size == new_size && Is2d()) {
    if (!canvas_is_clear_) {
      canvas_is_clear_ = true;
      context_->ClearRect(0, 0, width(), height());
    }
    return;
  }

  SetSurfaceSize(new_size);

  if (Is3d() && old_size != Size())
    context_->Reshape(width(), height());

  if (LayoutObject* layout_object = GetLayoutObject()) {
    if (layout_object->IsCanvas()) {
      if (old_size != Size()) {
        ToLayoutHTMLCanvas(layout_object)->CanvasSizeChanged();
        if (GetLayoutBox() && GetLayoutBox()->HasAcceleratedCompositing())
          GetLayoutBox()->ContentChanged(kCanvasChanged);
      }
      if (had_resource_provider)
        layout_object->SetShouldDoFullPaintInvalidation();
    }
  }
}
