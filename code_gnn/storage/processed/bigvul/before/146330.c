void WebGLRenderingContextBase::MarkContextChanged(
    ContentChangeType change_type) {
  if (framebuffer_binding_ || isContextLost())
    return;

  if (!GetDrawingBuffer()->MarkContentsChanged() && marked_canvas_dirty_) {
    return;
  }

  if (!canvas())
    return;

  marked_canvas_dirty_ = true;

  if (!animation_frame_in_progress_) {
    animation_frame_in_progress_ = true;
    LayoutBox* layout_box = canvas()->GetLayoutBox();
    if (layout_box && layout_box->HasAcceleratedCompositing()) {
      layout_box->ContentChanged(change_type);
    }
    IntSize canvas_size = ClampedCanvasSize();
    DidDraw(SkIRect::MakeXYWH(0, 0, canvas_size.Width(), canvas_size.Height()));
  }
}
