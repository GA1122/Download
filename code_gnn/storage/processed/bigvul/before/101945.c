void MockPrinter::ScriptedPrint(int cookie,
                                int expected_pages_count,
                                bool has_selection,
                                PrintMsg_PrintPages_Params* settings) {
  EXPECT_EQ(document_cookie_, cookie);

  settings->Reset();

  settings->params.dpi = dpi_;
  settings->params.max_shrink = max_shrink_;
  settings->params.min_shrink = min_shrink_;
  settings->params.desired_dpi = desired_dpi_;
  settings->params.selection_only = selection_only_;
  settings->params.document_cookie = document_cookie_;
  settings->params.page_size = page_size_;
  settings->params.printable_size = printable_size_;
  settings->params.is_first_request = is_first_request_;
  settings->params.preview_request_id = preview_request_id_;
  settings->params.display_header_footer = display_header_footer_;
  settings->params.date = date_;
  settings->params.title = title_;
  settings->params.url = url_;
  printer_status_ = PRINTER_PRINTING;
}
