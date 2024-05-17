PDFiumPage::Area PDFiumEngine::GetCharIndex(const pp::MouseInputEvent& event,
                                            int* page_index,
                                            int* char_index,
                                            int* form_type,
                                            PDFiumPage::LinkTarget* target) {
  pp::Point mouse_point = event.GetPosition();
  return GetCharIndex(mouse_point, page_index, char_index, form_type, target);
}
