FPDF_PAGE PDFiumEngine::Form_GetCurrentPage(FPDF_FORMFILLINFO* param,
                                            FPDF_DOCUMENT document) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  int index = engine->last_page_mouse_down_;
  if (index == -1) {
    index = engine->GetMostVisiblePage();
    if (index == -1) {
      NOTREACHED();
      return nullptr;
    }
  }

  return engine->pages_[index]->GetPage();
}
