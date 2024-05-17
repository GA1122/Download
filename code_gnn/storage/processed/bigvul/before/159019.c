void PDFiumEngine::SelectAll() {
  if (in_form_text_area_)
    return;

  SelectionChangeInvalidator selection_invalidator(this);

  selection_.clear();
  for (const auto& page : pages_) {
    if (page->available())
      selection_.push_back(PDFiumRange(page.get(), 0, page->GetCharCount()));
  }
}
