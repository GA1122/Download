PrepareFrameAndViewForPrint::PrepareFrameAndViewForPrint(
    const ViewMsg_Print_Params& print_params,
    WebFrame* frame,
    WebNode* node,
    WebView* web_view)
        : frame_(frame), web_view_(web_view), expected_pages_count_(0),
          use_browser_overlays_(true) {
  int dpi = static_cast<int>(print_params.dpi);
#if defined(OS_MACOSX)
  dpi = printing::kPointsPerInch;
#endif   
  print_canvas_size_.set_width(
      ConvertUnit(print_params.printable_size.width(), dpi,
                  print_params.desired_dpi));

  print_canvas_size_.set_height(
      ConvertUnit(print_params.printable_size.height(), dpi,
                  print_params.desired_dpi));

  gfx::Size print_layout_size(print_canvas_size_);
  print_layout_size.set_height(static_cast<int>(
      static_cast<double>(print_layout_size.height()) * 1.25));

  prev_view_size_ = web_view->size();

  web_view->resize(print_layout_size);

  WebNode node_to_print;
  if (node)
    node_to_print = *node;
  expected_pages_count_ = frame->printBegin(
      print_canvas_size_, node_to_print, static_cast<int>(print_params.dpi),
      &use_browser_overlays_);
}
