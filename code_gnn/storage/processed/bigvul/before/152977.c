void PDFiumEngine::Form_DoGoToAction(FPDF_FORMFILLINFO* param,
                                     int page_index,
                                     int zoom_mode,
                                     float* position_array,
                                     int size_of_array) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->ScrollToPage(page_index);
}
