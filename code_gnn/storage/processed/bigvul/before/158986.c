bool PDFiumEngine::OnKeyUp(const pp::KeyboardInputEvent& event) {
  if (last_page_mouse_down_ == -1)
    return false;

  FPDF_PAGE page = pages_[last_page_mouse_down_]->GetPage();
  if (in_form_text_area_)
    SetFormSelectedText(form_, page);

  return !!FORM_OnKeyUp(form_, page, event.GetKeyCode(), event.GetModifiers());
}
