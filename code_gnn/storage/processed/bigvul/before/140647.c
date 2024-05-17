void OutOfProcessInstance::ProcessPreviewPageInfo(const std::string& url,
                                                  int dst_page_index) {
  if (!IsPrintPreview())
    return;

  int src_page_index = ExtractPrintPreviewPageIndex(url);
  if (src_page_index < 1)
    return;

  preview_pages_info_.push(std::make_pair(url, dst_page_index));
  LoadAvailablePreviewPage();
}
