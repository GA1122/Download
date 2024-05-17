void PDFiumEngine::MoveRangeSelectionExtent(const pp::Point& extent) {
  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  GetCharIndex(extent, &page_index, &char_index, &form_type, &target);
  if (page_index < 0 || char_index < 0)
    return;

  SelectionChangeInvalidator selection_invalidator(this);
  if (range_selection_direction_ == RangeSelectionDirection::Right) {
    ExtendSelection(page_index, char_index);
    return;
  }

  selection_.clear();
  selection_.push_back(PDFiumRange(pages_[page_index].get(), char_index, 0));

  GetCharIndex(range_selection_base_, &page_index, &char_index, &form_type,
               &target);
  ExtendSelection(page_index, char_index);
}
