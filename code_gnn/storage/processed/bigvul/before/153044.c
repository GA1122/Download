int PDFiumEngine::GetRenderingFlags() const {
  int flags = FPDF_LCD_TEXT | FPDF_NO_CATCH;
  if (render_grayscale_)
    flags |= FPDF_GRAYSCALE;
  if (client_->IsPrintPreview())
    flags |= FPDF_PRINTING;
  if (render_annots_)
    flags |= FPDF_ANNOT;
  return flags;
}
