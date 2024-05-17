void PDFiumEngine::SearchUsingICU(const base::string16& term,
                                  bool case_sensitive,
                                  bool first_search,
                                  int character_to_start_searching_from,
                                  int current_page) {
  DCHECK(!term.empty());

  const int original_text_length = pages_[current_page]->GetCharCount();
  int text_length = original_text_length;
  if (character_to_start_searching_from) {
    text_length -= character_to_start_searching_from;
  } else if (!first_search && last_character_index_to_search_ != -1 &&
             current_page == last_page_to_search_) {
    text_length = last_character_index_to_search_;
  }
  if (text_length <= 0)
    return;

  base::string16 page_text;
  PDFiumAPIStringBufferAdapter<base::string16> api_string_adapter(
      &page_text, text_length + 1, false);
  unsigned short* data =
      reinterpret_cast<unsigned short*>(api_string_adapter.GetData());
  int written = FPDFText_GetText(pages_[current_page]->GetTextPage(),
                                 character_to_start_searching_from,
                                 text_length + 1, data);
  api_string_adapter.Close(written);

  std::vector<PDFEngine::Client::SearchStringResult> results =
      client_->SearchString(page_text.c_str(), term.c_str(), case_sensitive);
  for (const auto& result : results) {
    int temp_start = result.start_index + character_to_start_searching_from;
    int start = FPDFText_GetCharIndexFromTextIndex(
        pages_[current_page]->GetTextPage(), temp_start);
    int end = FPDFText_GetCharIndexFromTextIndex(
        pages_[current_page]->GetTextPage(), temp_start + result.length);

    if (temp_start + result.length == original_text_length) {
      DCHECK_EQ(-1, end);
      end = original_text_length;
    }
    DCHECK_LT(start, end);
    DCHECK_EQ(term.size(), static_cast<size_t>(end - start));
    AddFindResult(PDFiumRange(pages_[current_page].get(), start, end - start));
  }
}
