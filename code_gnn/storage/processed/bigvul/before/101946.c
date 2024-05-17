void MockPrinter::SetDefaultPrintSettings(const PrintMsg_Print_Params& params) {
  dpi_ = params.dpi;
  max_shrink_ = params.max_shrink;
  min_shrink_ = params.min_shrink;
  desired_dpi_ = params.desired_dpi;
  selection_only_ = params.selection_only;
  page_size_ = params.page_size;
  printable_size_ = params.printable_size;
  margin_left_ = params.margin_left;
  margin_top_ = params.margin_top;
  display_header_footer_ = params.display_header_footer;
  date_ = params.date;
  title_ = params.title;
  url_ = params.url;
}
