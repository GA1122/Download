pp::FloatRect PDFiumEngine::GetCharBounds(int page_index, int char_index) {
  DCHECK(page_index >= 0 && page_index < static_cast<int>(pages_.size()));
  return pages_[page_index]->GetCharBounds(char_index);
}
