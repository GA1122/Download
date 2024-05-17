void PrepareFrameAndViewForPrint::ResizeForPrinting() {
  gfx::Size print_layout_size(web_print_params_.printContentArea.width,
                              web_print_params_.printContentArea.height);
  print_layout_size.set_height(
      static_cast<int>(static_cast<double>(print_layout_size.height()) * 1.25));

  if (!frame())
    return;
  blink::WebView* web_view = frame_.view();
  if (blink::WebFrame* web_frame = web_view->mainFrame())
    prev_scroll_offset_ = web_frame->scrollOffset();
  prev_view_size_ = web_view->size();

  web_view->resize(print_layout_size);
}
