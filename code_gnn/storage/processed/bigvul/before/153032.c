pp::Rect PDFiumEngine::GetPageBoundsRect(int index) {
  return pages_[index]->rect();
}
