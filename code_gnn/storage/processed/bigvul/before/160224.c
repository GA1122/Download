void PDFiumEngine::TransformPDFPageForPrinting(
    FPDF_PAGE page,
    const PP_PrintSettings_Dev& print_settings) {
  const double src_page_width = FPDF_GetPageWidth(page);
  const double src_page_height = FPDF_GetPageHeight(page);

  const int src_page_rotation = FPDFPage_GetRotation(page);
  const bool fit_to_page = print_settings.print_scaling_option ==
                           PP_PRINTSCALINGOPTION_FIT_TO_PRINTABLE_AREA;

  pp::Size page_size(print_settings.paper_size);
  pp::Rect content_rect(print_settings.printable_area);
  const bool rotated = (src_page_rotation % 2 == 1);
  SetPageSizeAndContentRect(rotated, src_page_width > src_page_height,
                            &page_size, &content_rect);

  const int actual_page_width =
      rotated ? page_size.height() : page_size.width();
  const int actual_page_height =
      rotated ? page_size.width() : page_size.height();

  const gfx::Rect gfx_content_rect(content_rect.x(), content_rect.y(),
                                   content_rect.width(), content_rect.height());
  const double scale_factor =
      fit_to_page ? CalculateScaleFactor(gfx_content_rect, src_page_width,
                                         src_page_height, rotated)
                  : 1.0;

  PdfRectangle media_box;
  PdfRectangle crop_box;
  bool has_media_box =
      !!FPDFPage_GetMediaBox(page, &media_box.left, &media_box.bottom,
                             &media_box.right, &media_box.top);
  bool has_crop_box = !!FPDFPage_GetCropBox(
      page, &crop_box.left, &crop_box.bottom, &crop_box.right, &crop_box.top);
  CalculateMediaBoxAndCropBox(rotated, has_media_box, has_crop_box, &media_box,
                              &crop_box);
  PdfRectangle source_clip_box = CalculateClipBoxBoundary(media_box, crop_box);
  ScalePdfRectangle(scale_factor, &source_clip_box);

  double offset_x = 0;
  double offset_y = 0;
  if (fit_to_page) {
    CalculateScaledClipBoxOffset(gfx_content_rect, source_clip_box, &offset_x,
                                 &offset_y);
  } else {
    CalculateNonScaledClipBoxOffset(gfx_content_rect, src_page_rotation,
                                    actual_page_width, actual_page_height,
                                    source_clip_box, &offset_x, &offset_y);
  }

  FPDFPage_SetMediaBox(page, 0, 0, page_size.width(), page_size.height());
  FPDFPage_SetCropBox(page, 0, 0, page_size.width(), page_size.height());

  if (scale_factor == 1.0 && offset_x == 0 && offset_y == 0)
    return;

  FS_MATRIX matrix = {static_cast<float>(scale_factor),
                      0,
                      0,
                      static_cast<float>(scale_factor),
                      static_cast<float>(offset_x),
                      static_cast<float>(offset_y)};
  FS_RECTF cliprect = {static_cast<float>(source_clip_box.left + offset_x),
                       static_cast<float>(source_clip_box.top + offset_y),
                       static_cast<float>(source_clip_box.right + offset_x),
                       static_cast<float>(source_clip_box.bottom + offset_y)};
  FPDFPage_TransFormWithClip(page, &matrix, &cliprect);
  FPDFPage_TransformAnnots(page, scale_factor, 0, 0, scale_factor, offset_x,
                           offset_y);
}
