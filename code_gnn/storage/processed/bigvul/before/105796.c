void Label::OnPaint(gfx::Canvas* canvas) {
  OnPaintBackground(canvas);

  std::wstring paint_text;
  gfx::Rect text_bounds;
  int flags = 0;
  CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  PaintText(canvas, paint_text, text_bounds, flags);
}
