int PDFiumEngine::GetCharCount(int page_index) {
  DCHECK(page_index >= 0 && page_index < static_cast<int>(pages_.size()));
  return pages_[page_index]->GetCharCount();
}
