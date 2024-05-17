static void StrokeRectOnCanvas(const FloatRect& rect,
                               PaintCanvas* canvas,
                               const PaintFlags* flags) {
  DCHECK_EQ(flags->getStyle(), PaintFlags::kStroke_Style);
  if ((rect.Width() > 0) != (rect.Height() > 0)) {
    SkPath path;
    path.moveTo(rect.X(), rect.Y());
    path.lineTo(rect.MaxX(), rect.MaxY());
    path.close();
    canvas->drawPath(path, *flags);
    return;
  }
  canvas->drawRect(rect, *flags);
}
