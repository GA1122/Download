void PDFiumEngine::SetInFormTextArea(bool in_form_text_area) {
  if (in_form_text_area_)
    pp::PDF::SetSelectedText(GetPluginInstance(), "");

  client_->FormTextFieldFocusChange(in_form_text_area);
  in_form_text_area_ = in_form_text_area;

  if (!in_form_text_area_)
    editable_form_text_area_ = false;
}
