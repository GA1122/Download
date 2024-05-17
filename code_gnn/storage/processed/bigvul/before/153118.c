void PDFiumEngineExports::SetPDFEnsureTypefaceCharactersAccessible(
    PDFEnsureTypefaceCharactersAccessible func) {
  FPDF_SetTypefaceAccessibleFunc(
      reinterpret_cast<PDFiumEnsureTypefaceCharactersAccessible>(func));
}
