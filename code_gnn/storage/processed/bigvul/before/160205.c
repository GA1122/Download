void PDFiumEngine::Form_OnChange(FPDF_FORMFILLINFO* param) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->SetEditMode(true);
}
