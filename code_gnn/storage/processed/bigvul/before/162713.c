void BaseRenderingContext2D::clearRect(double x,
                                       double y,
                                       double width,
                                       double height) {
  usage_counters_.num_clear_rect_calls++;

  if (!ValidateRectForCanvas(x, y, width, height))
    return;

  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;
  if (!GetState().IsTransformInvertible())
    return;

  SkIRect clip_bounds;
  if (!c->getDeviceClipBounds(&clip_bounds))
    return;

  PaintFlags clear_flags;
  clear_flags.setBlendMode(SkBlendMode::kClear);
  clear_flags.setStyle(PaintFlags::kFill_Style);
  FloatRect rect(x, y, width, height);

  if (RectContainsTransformedRect(rect, clip_bounds)) {
    CheckOverdraw(rect, &clear_flags, CanvasRenderingContext2DState::kNoImage,
                  kClipFill);
    if (DrawingCanvas())
      DrawingCanvas()->drawRect(rect, clear_flags);
    DidDraw(clip_bounds);
  } else {
    SkIRect dirty_rect;
    if (ComputeDirtyRect(rect, clip_bounds, &dirty_rect)) {
      c->drawRect(rect, clear_flags);
      DidDraw(dirty_rect);
    }
  }
}
