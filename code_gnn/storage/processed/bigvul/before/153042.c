int PDFiumEngine::GetProgressiveIndex(int page_index) const {
  for (size_t i = 0; i < progressive_paints_.size(); ++i) {
    if (progressive_paints_[i].page_index == page_index)
      return i;
  }
  return -1;
}
