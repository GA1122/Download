void PDFiumEngine::RotateInternal() {
  std::string current_find_text = current_find_text_;
  resume_find_index_ = current_find_index_;

  int most_visible_page = most_visible_page_;

  InvalidateAllPages();

  if (!current_find_text.empty()) {
    client_->NotifyNumberOfFindResultsChanged(0, false);
    StartFind(current_find_text, false);
  }

  client_->ScrollToPage(most_visible_page);
}
