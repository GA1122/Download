void HeadlessPrintManager::Reset() {
  printing_rfh_ = nullptr;
  callback_.Reset();
  print_params_.reset();
  page_ranges_text_.clear();
  ignore_invalid_page_ranges_ = false;
  data_.clear();
}
