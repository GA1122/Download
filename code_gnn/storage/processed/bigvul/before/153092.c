void PDFiumEngine::PrintBegin() {
  FORM_DoDocumentAAction(form_, FPDFDOC_AACTION_WP);
}
