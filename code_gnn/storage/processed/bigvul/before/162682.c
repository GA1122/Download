void BaseRenderingContext2D::ClearCanvas() {
  FloatRect canvas_rect(0, 0, Width(), Height());
  CheckOverdraw(canvas_rect, nullptr, CanvasRenderingContext2DState::kNoImage,
                kClipFill);
  PaintCanvas* c = DrawingCanvas();
  if (c)
    c->clear(HasAlpha() ? SK_ColorTRANSPARENT : SK_ColorBLACK);
}
