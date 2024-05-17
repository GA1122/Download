void BrowserView::Paint1pxHorizontalLine(gfx::Canvas* canvas,
                                         SkColor color,
                                         const gfx::Rect& bounds,
                                         bool at_bottom) {
  gfx::ScopedCanvas scoped_canvas(canvas);
  const float scale = canvas->UndoDeviceScaleFactor();
  gfx::RectF rect(gfx::ScaleRect(gfx::RectF(bounds), scale));
  const float inset = rect.height() - 1;
  rect.Inset(0, at_bottom ? inset : 0, 0, at_bottom ? 0 : inset);
  cc::PaintFlags flags;
  flags.setColor(color);
  canvas->sk_canvas()->drawRect(gfx::RectFToSkRect(rect), flags);
}
