void PDFiumEngine::GetTextRunInfo(int page_index,
                                  int start_char_index,
                                  uint32_t* out_len,
                                  double* out_font_size,
                                  pp::FloatRect* out_bounds) {
  DCHECK(page_index >= 0 && page_index < static_cast<int>(pages_.size()));
  return pages_[page_index]->GetTextRunInfo(start_char_index, out_len,
                                            out_font_size, out_bounds);
}
