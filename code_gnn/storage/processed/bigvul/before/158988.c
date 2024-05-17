bool PDFiumEngine::OnMiddleMouseDown(const pp::MouseInputEvent& event) {
  SetMouseLeftButtonDown(false);

  SelectionChangeInvalidator selection_invalidator(this);
  selection_.clear();

  int unused_page_index = -1;
  int unused_char_index = -1;
  int unused_form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  PDFiumPage::Area area =
      GetCharIndex(event.GetPosition(), &unused_page_index, &unused_char_index,
                   &unused_form_type, &target);
  mouse_down_state_.Set(area, target);

  if (IsLinkArea(area))
    return true;

  return false;
}
