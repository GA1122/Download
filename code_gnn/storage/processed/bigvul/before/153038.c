bool PDFiumEngine::GetPageSizeAndUniformity(pp::Size* size) {
  if (pages_.empty())
    return false;

  pp::Size page_size = GetPageSize(0);
  for (size_t i = 1; i < pages_.size(); ++i) {
    if (page_size != GetPageSize(i))
      return false;
  }

  size->set_width(
      ConvertUnit(page_size.width(), kPixelsPerInch, kPointsPerInch));
  size->set_height(
      ConvertUnit(page_size.height(), kPixelsPerInch, kPointsPerInch));
  return true;
}
