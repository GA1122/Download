void PDFiumEngine::Form_GotoPage(IPDF_JSPLATFORM* param,
                                 int page_number) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->ScrollToPage(page_number);
}
