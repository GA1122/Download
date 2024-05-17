void PDFiumEngine::Form_SetCurrentPage(FPDF_FORMFILLINFO* param,
                                       FPDF_DOCUMENT document,
                                       int page) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  pp::Rect page_view_rect = engine->GetPageContentsRect(page);
  engine->ScrolledToYPosition(page_view_rect.height());
  pp::Point pos(1, page_view_rect.height());
  engine->SetScrollPosition(pos);
}
