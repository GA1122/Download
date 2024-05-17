void PrepareFrameAndViewForPrint::StartPrinting() {
  ResizeForPrinting();
  blink::WebView* web_view = frame_.view();
  web_view->settings()->setShouldPrintBackgrounds(should_print_backgrounds_);
  expected_pages_count_ =
      frame()->printBegin(web_print_params_, node_to_print_);
  is_printing_started_ = true;
}
