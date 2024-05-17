void PDFiumEngine::Form_OutputSelectedRect(FPDF_FORMFILLINFO* param,
                                           FPDF_PAGE page,
                                           double left,
                                           double top,
                                           double right,
                                           double bottom) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  int page_index = engine->GetVisiblePageIndex(page);
  if (page_index == -1) {
    NOTREACHED();
    return;
  }
  pp::Rect rect = engine->pages_[page_index]->PageToScreen(
      engine->GetVisibleRect().point(), engine->current_zoom_, left, top, right,
      bottom, engine->current_rotation_);
  if (rect.IsEmpty())
    return;

  engine->form_highlights_.push_back(rect);
}
