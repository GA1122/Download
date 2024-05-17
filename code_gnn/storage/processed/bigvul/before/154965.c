void WebGLRenderingContextBase::MarkContextChanged(
    ContentChangeType change_type) {
  if (isContextLost())
    return;

  if (framebuffer_binding_) {
    framebuffer_binding_->SetContentsChanged(true);
    return;
  }

  must_paint_to_canvas_ = true;

  if (!GetDrawingBuffer()->MarkContentsChanged() && marked_canvas_dirty_) {
    return;
  }

  if (Host()->IsOffscreenCanvas()) {
    marked_canvas_dirty_ = true;
    DidDraw();
    return;
  }

  if (!canvas())
    return;

  if (!marked_canvas_dirty_) {
    marked_canvas_dirty_ = true;
    LayoutBox* layout_box = canvas()->GetLayoutBox();
    if (layout_box && layout_box->HasAcceleratedCompositing()) {
      layout_box->ContentChanged(change_type);
    }
    IntSize canvas_size = ClampedCanvasSize();
    DidDraw(SkIRect::MakeXYWH(0, 0, canvas_size.Width(), canvas_size.Height()));
  }
}
