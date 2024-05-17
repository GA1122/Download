int PDFiumEngine::GetCopiesToPrint() {
  return FPDF_VIEWERREF_GetNumCopies(doc_);
}
