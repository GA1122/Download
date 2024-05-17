int PDFiumEngine::StartPaint(int page_index, const pp::Rect& dirty) {
  ProgressivePaint progressive;
  progressive.rect = dirty;
  progressive.page_index = page_index;
  progressive.bitmap = nullptr;
  progressive.painted_ = false;
  progressive_paints_.push_back(progressive);
  return progressive_paints_.size() - 1;
}
