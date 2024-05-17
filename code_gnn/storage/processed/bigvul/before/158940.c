pp::FloatRect PDFiumEngine::GetCharBounds(int page_index, int char_index) {
  DCHECK(PageIndexInBounds(page_index));
  return pages_[page_index]->GetCharBounds(char_index);
}
