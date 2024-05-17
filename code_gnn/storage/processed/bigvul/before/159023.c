void PDFiumEngine::SetFormSelectedText(FPDF_FORMHANDLE form_handle,
                                       FPDF_PAGE page) {
  unsigned long form_sel_text_len =
      FORM_GetSelectedText(form_handle, page, nullptr, 0);

  if (form_sel_text_len <= 2 && selected_form_text_.empty())
    return;

  base::string16 selected_form_text16;
  PDFiumAPIStringBufferSizeInBytesAdapter<base::string16> string_adapter(
      &selected_form_text16, form_sel_text_len, false);
  string_adapter.Close(FORM_GetSelectedText(
      form_handle, page, string_adapter.GetData(), form_sel_text_len));

  std::string selected_form_text = selected_form_text_;
  selected_form_text_ = base::UTF16ToUTF8(selected_form_text16);
  if (selected_form_text != selected_form_text_) {
    DCHECK(in_form_text_area_);
    pp::PDF::SetSelectedText(GetPluginInstance(), selected_form_text_.c_str());
  }
}
