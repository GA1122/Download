void PDFiumEngine::SetSelectionBounds(const pp::Point& base,
                                      const pp::Point& extent) {
  range_selection_base_ = base;
  range_selection_direction_ = IsAboveOrDirectlyLeftOf(base, extent)
                                   ? RangeSelectionDirection::Left
                                   : RangeSelectionDirection::Right;
}
