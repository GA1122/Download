void PDFiumEngine::PostPaint() {
  for (size_t i = 0; i < progressive_paints_.size(); ++i) {
    if (progressive_paints_[i].painted_)
      continue;

    FPDF_RenderPage_Close(
        pages_[progressive_paints_[i].page_index]->GetPage());
    FPDFBitmap_Destroy(progressive_paints_[i].bitmap);
    progressive_paints_.erase(progressive_paints_.begin() + i);
    --i;
  }
}
