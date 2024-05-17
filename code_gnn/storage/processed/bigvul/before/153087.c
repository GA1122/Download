void PDFiumEngine::PaintUnavailablePage(int page_index,
                                        const pp::Rect& dirty,
                                        pp::ImageData* image_data) {
  int start_x, start_y, size_x, size_y;
  GetPDFiumRect(page_index, dirty, &start_x, &start_y, &size_x, &size_y);
  FPDF_BITMAP bitmap = CreateBitmap(dirty, image_data);
  FPDFBitmap_FillRect(bitmap, start_x, start_y, size_x, size_y,
                      kPendingPageColor);

  pp::Rect loading_text_in_screen(
      pages_[page_index]->rect().width() / 2,
      pages_[page_index]->rect().y() + kLoadingTextVerticalOffset, 0, 0);
  loading_text_in_screen = GetScreenRect(loading_text_in_screen);
  FPDFBitmap_Destroy(bitmap);
}
