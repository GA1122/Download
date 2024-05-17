bool PrintWebViewHelper::PrintPages(WebFrame* frame, const WebNode& node) {
  const PrintMsg_PrintPages_Params& params = *print_pages_params_;
  const PrintMsg_Print_Params& print_params = params.params;
  PrepareFrameAndViewForPrint prep_frame_view(print_params, frame, node);
  UpdateFrameAndViewFromCssPageLayout(frame, node, &prep_frame_view,
                                      print_params, ignore_css_margins_);

  int page_count = prep_frame_view.GetExpectedPageCount();
  if (!page_count)
    return false;
  Send(new PrintHostMsg_DidGetPrintedPagesCount(routing_id(),
                                                print_params.document_cookie,
                                                page_count));

  const gfx::Size& canvas_size = prep_frame_view.GetPrintCanvasSize();
  PrintMsg_PrintPage_Params page_params;
  page_params.params = print_params;
  if (params.pages.empty()) {
    for (int i = 0; i < page_count; ++i) {
      page_params.page_number = i;
      PrintPageInternal(page_params, canvas_size, frame);
    }
  } else {
    for (size_t i = 0; i < params.pages.size(); ++i) {
      if (params.pages[i] >= page_count)
        break;
      page_params.page_number = params.pages[i];
      PrintPageInternal(page_params, canvas_size, frame);
    }
  }
  return true;
}
