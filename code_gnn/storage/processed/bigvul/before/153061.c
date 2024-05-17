bool PDFiumEngine::IsPageVisible(int index) const {
  return base::ContainsValue(visible_pages_, index);
}
