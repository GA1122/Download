bool PDFiumEngine::GetPrintScaling() {
  return !!FPDF_VIEWERREF_GetPrintScaling(doc_);
}
