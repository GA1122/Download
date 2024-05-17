void PDFiumEngine::PrePaint() {
  for (auto& paint : progressive_paints_)
    paint.painted_ = false;
}
