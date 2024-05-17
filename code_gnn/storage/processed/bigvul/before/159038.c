PDFiumEngine::~PDFiumEngine() {
  for (auto& page : pages_)
    page->Unload();

  if (doc_) {
    FORM_DoDocumentAAction(form_, FPDFDOC_AACTION_WC);
    FPDFDOC_ExitFormFillEnvironment(form_);
    FPDF_CloseDocument(doc_);
  }
  FPDFAvail_Destroy(fpdf_availability_);
}
