FPDF_PAGE PDFiumEngine::Form_GetPage(FPDF_FORMFILLINFO* param,
                                     FPDF_DOCUMENT document,
                                     int page_index) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  if (page_index < 0 || page_index >= static_cast<int>(engine->pages_.size()))
    return nullptr;
  return engine->pages_[page_index]->GetPage();
}
