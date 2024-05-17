bool PDFiumEngine::OnMouseDown(const pp::MouseInputEvent& event) {
  if (event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_RIGHT) {
    if (selection_.empty())
      return false;
    std::vector<pp::Rect> selection_rect_vector;
    GetAllScreenRectsUnion(&selection_, GetVisibleRect().point(),
                           &selection_rect_vector);
    pp::Point point = event.GetPosition();
    for (const auto& rect : selection_rect_vector) {
      if (rect.Contains(point.x(), point.y()))
        return false;
    }
    SelectionChangeInvalidator selection_invalidator(this);
    selection_.clear();
    return true;
  }

  if (event.GetButton() != PP_INPUTEVENT_MOUSEBUTTON_LEFT &&
      event.GetButton() != PP_INPUTEVENT_MOUSEBUTTON_MIDDLE) {
    return false;
  }

  SelectionChangeInvalidator selection_invalidator(this);
  selection_.clear();

  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  PDFiumPage::Area area =
      GetCharIndex(event, &page_index, &char_index, &form_type, &target);
  mouse_down_state_.Set(area, target);

  if (area == PDFiumPage::WEBLINK_AREA)
    return true;

  if (event.GetButton() == PP_INPUTEVENT_MOUSEBUTTON_MIDDLE)
    return false;

  if (area == PDFiumPage::DOCLINK_AREA) {
    client_->ScrollToPage(target.page);
    client_->FormTextFieldFocusChange(false);
    return true;
  }

  if (page_index != -1) {
    last_page_mouse_down_ = page_index;
    double page_x, page_y;
    pp::Point point = event.GetPosition();
    DeviceToPage(page_index, point.x(), point.y(), &page_x, &page_y);

    FORM_OnLButtonDown(form_, pages_[page_index]->GetPage(), 0, page_x, page_y);
    if (form_type > FPDF_FORMFIELD_UNKNOWN) {   
      mouse_down_state_.Set(PDFiumPage::NONSELECTABLE_AREA, target);
      bool is_valid_control = (form_type == FPDF_FORMFIELD_TEXTFIELD ||
                               form_type == FPDF_FORMFIELD_COMBOBOX);
#if defined(PDF_ENABLE_XFA)
      is_valid_control |= (form_type == FPDF_FORMFIELD_XFA);
#endif
      client_->FormTextFieldFocusChange(is_valid_control);
      return true;   
    }
  }

  client_->FormTextFieldFocusChange(false);

  if (area != PDFiumPage::TEXT_AREA)
    return true;   

  if (event.GetClickCount() == 1) {
    OnSingleClick(page_index, char_index);
  } else if (event.GetClickCount() == 2 ||
             event.GetClickCount() == 3) {
    OnMultipleClick(event.GetClickCount(), page_index, char_index);
  }

  return true;
}
