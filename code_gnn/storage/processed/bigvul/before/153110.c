void PDFiumEngine::SearchUsingPDFium(const base::string16& term,
                                     bool case_sensitive,
                                     bool first_search,
                                     int character_to_start_searching_from,
                                     int current_page) {
  unsigned long flags = case_sensitive ? FPDF_MATCHCASE : 0;
  FPDF_SCHHANDLE find = FPDFText_FindStart(
      pages_[current_page]->GetTextPage(),
      reinterpret_cast<const unsigned short*>(term.c_str()),
      flags, character_to_start_searching_from);

  while (FPDFText_FindNext(find)) {
    PDFiumRange result(pages_[current_page],
                       FPDFText_GetSchResultIndex(find),
                       FPDFText_GetSchCount(find));

    if (!first_search &&
        last_character_index_to_search_ != -1 &&
        result.page_index() == last_page_to_search_ &&
        result.char_index() >= last_character_index_to_search_) {
      break;
    }

    AddFindResult(result);
  }

  FPDFText_FindClose(find);
}
