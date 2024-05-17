void PDFiumEngine::ReplaceSelection(const std::string& text) {
  DCHECK(CanEditText());
  if (last_page_mouse_down_ != -1) {
    base::string16 text_wide = base::UTF8ToUTF16(text);
    FPDF_WIDESTRING text_pdf_wide =
        reinterpret_cast<FPDF_WIDESTRING>(text_wide.c_str());

    FORM_ReplaceSelection(form_, pages_[last_page_mouse_down_]->GetPage(),
                          text_pdf_wide);
  }
}
