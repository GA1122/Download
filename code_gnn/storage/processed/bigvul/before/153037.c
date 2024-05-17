pp::Size PDFiumEngine::GetPageSize(int index) {
  pp::Size size;
  double width_in_points = 0;
  double height_in_points = 0;
  int rv = FPDF_GetPageSizeByIndex(
      doc_, index, &width_in_points, &height_in_points);

  if (rv) {
    int width_in_pixels = static_cast<int>(
        ConvertUnitDouble(width_in_points, kPointsPerInch, kPixelsPerInch));
    int height_in_pixels = static_cast<int>(
        ConvertUnitDouble(height_in_points, kPointsPerInch, kPixelsPerInch));
    if (current_rotation_ % 2 == 1)
      std::swap(width_in_pixels, height_in_pixels);
    size = pp::Size(width_in_pixels, height_in_pixels);
  }
  return size;
}
