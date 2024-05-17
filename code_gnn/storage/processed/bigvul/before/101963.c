bool PrintWebViewHelper::PrintPreviewContext::IsLastPageOfPrintReadyMetafile()
    const {
  return current_page_index_ == print_ready_metafile_page_count_;
}
