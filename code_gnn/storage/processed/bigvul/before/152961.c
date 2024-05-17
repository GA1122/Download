FPDF_DOCUMENT PDFiumEngine::CreateSinglePageRasterPdf(
    double source_page_width,
    double source_page_height,
    const PP_PrintSettings_Dev& print_settings,
    PDFiumPage* page_to_print) {
  FPDF_DOCUMENT temp_doc = FPDF_CreateNewDocument();
  if (!temp_doc)
    return temp_doc;

  const pp::Size& bitmap_size(page_to_print->rect().size());

  FPDF_PAGE temp_page =
      FPDFPage_New(temp_doc, 0, source_page_width, source_page_height);

  pp::ImageData image = pp::ImageData(client_->GetPluginInstance(),
                                      PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                                      bitmap_size,
                                      false);

  FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(bitmap_size.width(),
                                           bitmap_size.height(),
                                           FPDFBitmap_BGRx,
                                           image.data(),
                                           image.stride());

  FPDFBitmap_FillRect(
      bitmap, 0, 0, bitmap_size.width(), bitmap_size.height(), 0xFFFFFFFF);

  pp::Rect page_rect = page_to_print->rect();
  FPDF_RenderPageBitmap(bitmap,
                        page_to_print->GetPrintPage(),
                        page_rect.x(),
                        page_rect.y(),
                        page_rect.width(),
                        page_rect.height(),
                        print_settings.orientation,
                        FPDF_ANNOT | FPDF_PRINTING | FPDF_NO_CATCH);

  double ratio_x = ConvertUnitDouble(bitmap_size.width(),
                                     print_settings.dpi,
                                     kPointsPerInch);
  double ratio_y = ConvertUnitDouble(bitmap_size.height(),
                                     print_settings.dpi,
                                     kPointsPerInch);

  FPDF_PAGEOBJECT temp_img = FPDFPageObj_NewImgeObj(temp_doc);
  FPDFImageObj_SetBitmap(&temp_page, 1, temp_img, bitmap);
  FPDFImageObj_SetMatrix(temp_img, ratio_x, 0, 0, ratio_y, 0, 0);
  FPDFPage_InsertObject(temp_page, temp_img);
  FPDFPage_GenerateContent(temp_page);
  FPDF_ClosePage(temp_page);

  page_to_print->ClosePrintPage();
  FPDFBitmap_Destroy(bitmap);

  return temp_doc;
}
