bool PDFiumEngine::OnMouseMove(const pp::MouseInputEvent& event) {
  int page_index = -1;
  int char_index = -1;
  int form_type = FPDF_FORMFIELD_UNKNOWN;
  PDFiumPage::LinkTarget target;
  PDFiumPage::Area area =
      GetCharIndex(event, &page_index, &char_index, &form_type, &target);

  if (!mouse_down_state_.Matches(area, target))
    mouse_down_state_.Reset();

  if (!selecting_) {
    PP_CursorType_Dev cursor;
    switch (area) {
      case PDFiumPage::TEXT_AREA:
        cursor = PP_CURSORTYPE_IBEAM;
        break;
      case PDFiumPage::WEBLINK_AREA:
      case PDFiumPage::DOCLINK_AREA:
        cursor = PP_CURSORTYPE_HAND;
        break;
      case PDFiumPage::NONSELECTABLE_AREA:
      default:
        switch (form_type) {
          case FPDF_FORMFIELD_PUSHBUTTON:
          case FPDF_FORMFIELD_CHECKBOX:
          case FPDF_FORMFIELD_RADIOBUTTON:
          case FPDF_FORMFIELD_COMBOBOX:
          case FPDF_FORMFIELD_LISTBOX:
            cursor = PP_CURSORTYPE_HAND;
            break;
          case FPDF_FORMFIELD_TEXTFIELD:
            cursor = PP_CURSORTYPE_IBEAM;
            break;
          default:
            cursor = PP_CURSORTYPE_POINTER;
            break;
        }
        break;
    }

    if (page_index != -1) {
      double page_x, page_y;
      pp::Point point = event.GetPosition();
      DeviceToPage(page_index, point.x(), point.y(), &page_x, &page_y);
      FORM_OnMouseMove(form_, pages_[page_index]->GetPage(), 0, page_x, page_y);
    }

    client_->UpdateCursor(cursor);
    std::string url = GetLinkAtPosition(event.GetPosition());
    if (url != link_under_cursor_) {
      link_under_cursor_ = url;
      pp::PDF::SetLinkUnderCursor(GetPluginInstance(), url.c_str());
    }
    return false;
  }

  if (area != PDFiumPage::TEXT_AREA && area != PDFiumPage::WEBLINK_AREA &&
      area != PDFiumPage::DOCLINK_AREA) {
    return false;
  }

  SelectionChangeInvalidator selection_invalidator(this);

  for (size_t i = 0; i < selection_.size(); ++i) {
    if (selection_[i].page_index() == page_index) {
      selection_.erase(selection_.begin() + i + 1, selection_.end());
      break;
    }
  }

  if (selection_.empty())
    return false;

  int last = selection_.size() - 1;
  if (selection_[last].page_index() == page_index) {
    int count;
    if (char_index >= selection_[last].char_index()) {
      count = char_index - selection_[last].char_index() + 1;
    } else {
      count = char_index - selection_[last].char_index() - 1;
    }
    selection_[last].SetCharCount(count);
  } else if (selection_[last].page_index() < page_index) {

    for (int i = selection_[last].page_index() + 1; i < page_index; ++i) {
      selection_.push_back(PDFiumRange(pages_[i], 0,
                           pages_[i]->GetCharCount()));
    }

    int count = pages_[selection_[last].page_index()]->GetCharCount();
    selection_[last].SetCharCount(count - selection_[last].char_index());
    selection_.push_back(PDFiumRange(pages_[page_index], 0, char_index));
  } else {
    selection_[last].SetCharCount(-(selection_[last].char_index() + 1));

    for (int i = selection_[last].page_index() - 1; i > page_index; --i) {
      selection_.push_back(PDFiumRange(pages_[i], 0,
                           pages_[i]->GetCharCount()));
    }

    int count = pages_[page_index]->GetCharCount();
    selection_.push_back(
        PDFiumRange(pages_[page_index], count, count - char_index));
  }

  return true;
}
