int PDFiumEngine::Form_GetCurrentPageIndex(FPDF_FORMFILLINFO* param,
                                           FPDF_DOCUMENT document) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  return engine->GetMostVisiblePage();
}
