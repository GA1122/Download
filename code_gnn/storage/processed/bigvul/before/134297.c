void OmniboxViewViews::SetForcedQuery() {
  const base::string16 current_text(text());
  const size_t start = current_text.find_first_not_of(base::kWhitespaceUTF16);
  if (start == base::string16::npos || (current_text[start] != '?'))
    OmniboxView::SetUserText(base::ASCIIToUTF16("?"));
  else
    SelectRange(gfx::Range(current_text.size(), start + 1));
}
