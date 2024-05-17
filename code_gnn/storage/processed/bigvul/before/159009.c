bool PDFiumEngineExports::RenderPDFPageToDC(const void* pdf_buffer,
                                            int buffer_size,
                                            int page_number,
                                            const RenderingSettings& settings,
                                            HDC dc) {
  FPDF_DOCUMENT doc = FPDF_LoadMemDocument(pdf_buffer, buffer_size, nullptr);
  if (!doc)
    return false;
  FPDF_PAGE page = FPDF_LoadPage(doc, page_number);
  if (!page) {
    FPDF_CloseDocument(doc);
    return false;
  }
  RenderingSettings new_settings = settings;
  if (new_settings.dpi_x == -1)
    new_settings.dpi_x = GetDeviceCaps(dc, LOGPIXELSX);
  if (new_settings.dpi_y == -1)
    new_settings.dpi_y = GetDeviceCaps(dc, LOGPIXELSY);

  pp::Rect dest;
  int rotate = CalculatePosition(page, new_settings, &dest);

  int save_state = SaveDC(dc);
  IntersectClipRect(dc, settings.bounds.x(), settings.bounds.y(),
                    settings.bounds.x() + settings.bounds.width(),
                    settings.bounds.y() + settings.bounds.height());

  int device_type = GetDeviceCaps(dc, TECHNOLOGY);
  if (device_type == DT_RASPRINTER || device_type == DT_PLOTTER) {
    FPDF_BITMAP bitmap =
        FPDFBitmap_Create(dest.width(), dest.height(), FPDFBitmap_BGRx);
    FPDFBitmap_FillRect(bitmap, 0, 0, dest.width(), dest.height(), 0xFFFFFFFF);
    FPDF_RenderPageBitmap(bitmap, page, 0, 0, dest.width(), dest.height(),
                          rotate, FPDF_ANNOT | FPDF_PRINTING | FPDF_NO_CATCH);
    int stride = FPDFBitmap_GetStride(bitmap);
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = dest.width();
    bmi.bmiHeader.biHeight = -dest.height();   
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = stride * dest.height();
    StretchDIBits(dc, dest.x(), dest.y(), dest.width(), dest.height(), 0, 0,
                  dest.width(), dest.height(), FPDFBitmap_GetBuffer(bitmap),
                  &bmi, DIB_RGB_COLORS, SRCCOPY);
    FPDFBitmap_Destroy(bitmap);
  } else {
    FPDF_RenderPage(dc, page, dest.x(), dest.y(), dest.width(), dest.height(),
                    rotate, FPDF_ANNOT | FPDF_PRINTING | FPDF_NO_CATCH);
  }
  RestoreDC(dc, save_state);
  FPDF_ClosePage(page);
  FPDF_CloseDocument(doc);
  return true;
}
