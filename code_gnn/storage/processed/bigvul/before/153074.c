bool PDFiumEngine::OnKeyUp(const pp::KeyboardInputEvent& event) {
  if (last_page_mouse_down_ == -1)
    return false;

  return !!FORM_OnKeyUp(
      form_, pages_[last_page_mouse_down_]->GetPage(),
      event.GetKeyCode(), event.GetModifiers());
}
