int CalculatePosition(FPDF_PAGE page,
                      const PDFiumEngineExports::RenderingSettings& settings,
                      pp::Rect* dest) {
  int page_width = static_cast<int>(ConvertUnitDouble(
      FPDF_GetPageWidth(page), kPointsPerInch, settings.dpi_x));
  int page_height = static_cast<int>(ConvertUnitDouble(
      FPDF_GetPageHeight(page), kPointsPerInch, settings.dpi_y));

  *dest = settings.bounds;

  int rotate = 0;   

  if (settings.autorotate &&
      (dest->width() > dest->height()) != (page_width > page_height)) {
    rotate = 3;   
    std::swap(page_width, page_height);
  }

  bool scale_to_bounds = false;
  if (settings.fit_to_bounds &&
      ((page_width > dest->width()) || (page_height > dest->height()))) {
    scale_to_bounds = true;
  } else if (settings.stretch_to_bounds &&
             ((page_width < dest->width()) || (page_height < dest->height()))) {
    scale_to_bounds = true;
  }

  if (scale_to_bounds) {
    if (settings.keep_aspect_ratio) {
      double scale_factor_x = page_width;
      scale_factor_x /= dest->width();
      double scale_factor_y = page_height;
      scale_factor_y /= dest->height();
      if (scale_factor_x > scale_factor_y) {
        dest->set_height(page_height / scale_factor_x);
      } else {
        dest->set_width(page_width / scale_factor_y);
      }
    }
  } else {
    dest->set_width(page_width);
    dest->set_height(page_height);
  }

  if (settings.center_in_bounds) {
    pp::Point offset((settings.bounds.width() - dest->width()) / 2,
                     (settings.bounds.height() - dest->height()) / 2);
    dest->Offset(offset);
  }
  return rotate;
}
