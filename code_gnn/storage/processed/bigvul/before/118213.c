void DrawArrow(gfx::Canvas* canvas,
               int tip_x,
               const SkColor& fill_color,
               const SkColor& stroke_color) {
  const int arrow_half_width = kArrowWidth / 2.0f;

  SkPath arrow;
  arrow.moveTo(tip_x - arrow_half_width, kArrowHeight);
  arrow.lineTo(tip_x, 0);
  arrow.lineTo(tip_x + arrow_half_width, kArrowHeight);

  SkPaint fill_paint;
  fill_paint.setColor(fill_color);
  canvas->DrawPath(arrow, fill_paint);

  if (stroke_color != SK_ColorTRANSPARENT) {
    SkPaint stroke_paint;
    stroke_paint.setColor(stroke_color);
    stroke_paint.setStyle(SkPaint::kStroke_Style);
    canvas->DrawPath(arrow, stroke_paint);
  }
}
