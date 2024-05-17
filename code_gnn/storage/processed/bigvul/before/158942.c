PDFiumPage::Area PDFiumEngine::GetCharIndex(const pp::Point& point,
                                            int* page_index,
                                            int* char_index,
                                            int* form_type,
                                            PDFiumPage::LinkTarget* target) {
  int page = -1;
  pp::Point point_in_page(
      static_cast<int>((point.x() + position_.x()) / current_zoom_),
      static_cast<int>((point.y() + position_.y()) / current_zoom_));
  for (int visible_page : visible_pages_) {
    if (pages_[visible_page]->rect().Contains(point_in_page)) {
      page = visible_page;
      break;
    }
  }
  if (page == -1)
    return PDFiumPage::NONSELECTABLE_AREA;

  for (const auto& paint : progressive_paints_) {
    if (paint.page_index == page)
      return PDFiumPage::NONSELECTABLE_AREA;
  }

  *page_index = page;
  PDFiumPage::Area result = pages_[page]->GetCharIndex(
      point_in_page, current_rotation_, char_index, form_type, target);
  return (client_->IsPrintPreview() && result == PDFiumPage::WEBLINK_AREA)
             ? PDFiumPage::NONSELECTABLE_AREA
             : result;
}
