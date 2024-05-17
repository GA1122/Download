void PrepareFrameAndViewForPrint::StartPrinting(
    const WebPrintParams& webkit_print_params) {
  web_print_params_ = webkit_print_params;

  gfx::Size print_layout_size(web_print_params_.printContentArea.width,
                              web_print_params_.printContentArea.height);
  print_layout_size.set_height(static_cast<int>(
      static_cast<double>(print_layout_size.height()) * 1.25));

  web_view_->resize(print_layout_size);

  expected_pages_count_ = frame_->printBegin(web_print_params_,
                                             node_to_print_,
                                             &use_browser_overlays_);
}
