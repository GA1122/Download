PrepareFrameAndViewForPrint::PrepareFrameAndViewForPrint(
    const PrintMsg_Print_Params& print_params,
    WebFrame* frame,
    const WebNode& node)
        : frame_(frame),
          node_to_print_(node),
          web_view_(frame->view()),
          expected_pages_count_(0),
          use_browser_overlays_(true),
          finished_(false) {
  WebPrintParams webkit_print_params;
  ComputeWebKitPrintParamsInDesiredDpi(print_params, &webkit_print_params);

  if (WebFrame* web_frame = web_view_->mainFrame())
    prev_scroll_offset_ = web_frame->scrollOffset();
  prev_view_size_ = web_view_->size();

  StartPrinting(webkit_print_params);
}
