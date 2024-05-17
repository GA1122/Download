PDFiumEngine::~PDFiumEngine() {
  for (auto& page : pages_)
    page->Unload();

  if (doc_) {
    FORM_DoDocumentAAction(form_, FPDFDOC_AACTION_WC);

#if defined(PDF_ENABLE_XFA)
    FPDF_CloseDocument(doc_);
    FPDFDOC_ExitFormFillEnvironment(form_);
#else
    FPDFDOC_ExitFormFillEnvironment(form_);
    FPDF_CloseDocument(doc_);
#endif
  }
  FPDFAvail_Destroy(fpdf_availability_);

  base::STLDeleteElements(&pages_);
}
