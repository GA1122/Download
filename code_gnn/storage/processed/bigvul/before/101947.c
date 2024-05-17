void MockPrinter::SetPrintParams(PrintMsg_Print_Params* params) {
  params->dpi = dpi_;
  params->max_shrink = max_shrink_;
  params->min_shrink = min_shrink_;
  params->desired_dpi = desired_dpi_;
  params->selection_only = selection_only_;
  params->document_cookie = document_cookie_;
  params->page_size = page_size_;
  params->printable_size = printable_size_;
  params->margin_left = margin_left_;
  params->margin_top = margin_top_;
  params->is_first_request = is_first_request_;
  params->preview_request_id = preview_request_id_;
  params->display_header_footer = display_header_footer_;
  params->date = date_;
  params->title = title_;
  params->url = url_;
}
