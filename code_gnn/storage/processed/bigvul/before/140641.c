void OutOfProcessInstance::PreviewDocumentLoadComplete() {
  if (preview_document_load_state_ != LOAD_STATE_LOADING ||
      preview_pages_info_.empty()) {
    return;
  }

  preview_document_load_state_ = LOAD_STATE_COMPLETE;

  int dest_page_index = preview_pages_info_.front().second;
  int src_page_index =
      ExtractPrintPreviewPageIndex(preview_pages_info_.front().first);
  if (src_page_index > 0 &&  dest_page_index > -1 && preview_engine_.get())
    engine_->AppendPage(preview_engine_.get(), dest_page_index);

  preview_pages_info_.pop();
  if (print_preview_page_count_ == 0)
    return;

  if (!preview_pages_info_.empty())
    LoadAvailablePreviewPage();
}
