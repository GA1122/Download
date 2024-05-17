FPDF_BITMAP PDFiumEngine::CreateBitmap(const pp::Rect& rect,
                                       pp::ImageData* image_data) const {
  void* region;
  int stride;
  GetRegion(rect.point(), image_data, &region, &stride);
  if (!region)
    return nullptr;
  return FPDFBitmap_CreateEx(rect.width(), rect.height(), FPDFBitmap_BGRx,
                             region, stride);
}
