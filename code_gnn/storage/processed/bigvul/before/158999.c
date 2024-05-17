bool PDFiumEngine::PageIndexInBounds(int index) const {
  return index >= 0 && index < static_cast<int>(pages_.size());
}
