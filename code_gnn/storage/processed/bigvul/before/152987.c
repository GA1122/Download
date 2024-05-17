void PDFiumEngine::Form_GetPageViewRect(FPDF_FORMFILLINFO* param,
                                        FPDF_PAGE page,
                                        double* left,
                                        double* top,
                                        double* right,
                                        double* bottom) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  int page_index = engine->GetMostVisiblePage();
  pp::Rect page_view_rect = engine->GetPageContentsRect(page_index);

  *left = page_view_rect.x();
  *right = page_view_rect.right();
  *top = page_view_rect.y();
  *bottom = page_view_rect.bottom();
}
