void PDFiumEngine::GetPDFiumRect(
    int page_index, const pp::Rect& rect, int* start_x, int* start_y,
    int* size_x, int* size_y) const {
  pp::Rect page_rect = GetScreenRect(pages_[page_index]->rect());
  page_rect.Offset(-rect.x(), -rect.y());

  *start_x = page_rect.x();
  *start_y = page_rect.y();
  *size_x = page_rect.width();
  *size_y = page_rect.height();
}
