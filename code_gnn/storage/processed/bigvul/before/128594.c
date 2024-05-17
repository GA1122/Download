void Instance::LoadAvailablePreviewPage() {
  if (preview_pages_info_.size() <= 0)
    return;

  std::string url = preview_pages_info_.front().first;
  int dst_page_index = preview_pages_info_.front().second;
  int src_page_index = ExtractPrintPreviewPageIndex(url);
  if (src_page_index < 1 ||
      dst_page_index >= print_preview_page_count_ ||
      preview_document_load_state_ == LOAD_STATE_LOADING) {
    return;
  }

  preview_document_load_state_ = LOAD_STATE_LOADING;
  LoadPreviewUrl(url);
}
