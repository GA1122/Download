bool PDFiumEngine::OnRightMouseDown(const pp::MouseInputEvent& event) {
  DCHECK_EQ(PP_INPUTEVENT_MOUSEBUTTON_RIGHT, event.GetButton());

  pp::Point point = event.GetPosition();
  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  PDFiumPage::Area area =
      GetCharIndex(point, &page_index, &char_index, &form_type, &target);
  DCHECK_GE(form_type, FPDF_FORMFIELD_UNKNOWN);

  bool is_form_text_area = IsFormTextArea(area, form_type);
  bool is_editable_form_text_area = false;

  double page_x = -1;
  double page_y = -1;
  FPDF_PAGE page = nullptr;
  if (is_form_text_area) {
    DCHECK_NE(page_index, -1);

    DeviceToPage(page_index, point.x(), point.y(), &page_x, &page_y);
    page = pages_[page_index]->GetPage();
    is_editable_form_text_area =
        IsPointInEditableFormTextArea(page, page_x, page_y, form_type);
  }

  if (in_form_text_area_) {
    if (is_form_text_area) {
      FORM_OnFocus(form_, page, 0, page_x, page_y);
    } else {
      FORM_ForceToKillFocus(form_);
      SetInFormTextArea(false);
    }
    return true;
  }

  if (is_form_text_area) {
    {
      SelectionChangeInvalidator selection_invalidator(this);
      selection_.clear();
    }

    SetInFormTextArea(true);
    editable_form_text_area_ = is_editable_form_text_area;
    FORM_OnFocus(form_, page, 0, page_x, page_y);
    return true;
  }

  if (selection_.empty())
    return false;

  std::vector<pp::Rect> selection_rect_vector;
  GetAllScreenRectsUnion(&selection_, GetVisibleRect().point(),
                         &selection_rect_vector);
  for (const auto& rect : selection_rect_vector) {
    if (rect.Contains(point.x(), point.y()))
      return false;
  }
  SelectionChangeInvalidator selection_invalidator(this);
  selection_.clear();
  return true;
}
