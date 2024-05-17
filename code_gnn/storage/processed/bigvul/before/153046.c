std::string PDFiumEngine::GetSelectedText() {
  if (!HasPermission(PDFEngine::PERMISSION_COPY))
    return std::string();

  base::string16 result;
  base::string16 new_line_char = base::UTF8ToUTF16("\n");
  for (size_t i = 0; i < selection_.size(); ++i) {
    if (i > 0 &&
        selection_[i - 1].page_index() > selection_[i].page_index()) {
      result = selection_[i].GetText() + new_line_char + result;
    } else {
      if (i > 0)
        result.append(new_line_char);
      result.append(selection_[i].GetText());
    }
  }

  FormatStringWithHyphens(&result);
  FormatStringForOS(&result);
  return base::UTF16ToUTF8(result);
}
