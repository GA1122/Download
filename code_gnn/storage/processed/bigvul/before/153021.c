int PDFiumEngine::GetDuplexType() {
  return static_cast<int>(FPDF_VIEWERREF_GetDuplex(doc_));
}
