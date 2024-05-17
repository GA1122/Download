void PDFiumEngine::SelectAll() {
  SelectionChangeInvalidator selection_invalidator(this);

  selection_.clear();
  for (const auto& page : pages_) {
    if (page->available())
      selection_.push_back(PDFiumRange(page, 0, page->GetCharCount()));
  }
}
