void PrepareFrameAndViewForPrint::ResizeForPrinting() {
  gfx::Size print_layout_size(web_print_params_.print_content_area.width,
                              web_print_params_.print_content_area.height);
  print_layout_size.set_height(
      ScaleAndRound(print_layout_size.height(), kPrintingMinimumShrinkFactor));

  if (!frame())
    return;

  if (PrintingNodeOrPdfFrame(frame(), node_to_print_))
    return;

  blink::WebView* web_view = frame_.view();
  if (blink::WebFrame* web_frame = web_view->MainFrame()) {
    if (web_frame->IsWebLocalFrame())
      prev_scroll_offset_ = web_frame->ToWebLocalFrame()->GetScrollOffset();
  }
  prev_view_size_ = web_view->Size();
  web_view->Resize(print_layout_size);
}
