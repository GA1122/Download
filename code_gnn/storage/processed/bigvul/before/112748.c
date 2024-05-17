void PrintPreviewHandler::HandleClosePreviewTab(const ListValue*  ) {
  ReportStats();
  ReportUserActionHistogram(CANCEL);

  UMA_HISTOGRAM_COUNTS("PrintPreview.RegeneratePreviewRequest.BeforeCancel",
                       regenerate_preview_request_count_);
}
