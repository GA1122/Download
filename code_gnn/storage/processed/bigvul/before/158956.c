std::string PDFiumEngine::GetSelectedText() {
  if (!HasPermission(PDFEngine::PERMISSION_COPY))
    return std::string();

  base::string16 result;
  for (size_t i = 0; i < selection_.size(); ++i) {
    static constexpr base::char16 kNewLineChar = L'\n';
    base::string16 current_selection_text = selection_[i].GetText();
    if (i != 0) {
      if (selection_[i - 1].page_index() > selection_[i].page_index())
        std::swap(current_selection_text, result);
      result.push_back(kNewLineChar);
    }
    result.append(current_selection_text);
  }

  FormatStringWithHyphens(&result);
  FormatStringForOS(&result);
  return base::UTF16ToUTF8(result);
}
