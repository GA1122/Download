void PDFiumEngine::CancelPaints() {
  for (const auto& paint : progressive_paints_) {
    FPDF_RenderPage_Close(pages_[paint.page_index]->GetPage());
    FPDFBitmap_Destroy(paint.bitmap);
  }
  progressive_paints_.clear();
}
