void PDFiumEngine::ScrollToPage(int page) {
  if (!PageIndexInBounds(page))
    return;

  in_flight_visible_page_ = page;
  client_->ScrollToPage(page);
}
