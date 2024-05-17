void PrepareFrameAndViewForPrint::RestoreSize() {
  if (!frame())
    return;

  if (PrintingNodeOrPdfFrame(frame(), node_to_print_))
    return;

  blink::WebView* web_view = frame_.GetFrame()->View();
  web_view->Resize(prev_view_size_);
  if (blink::WebFrame* web_frame = web_view->MainFrame()) {
    if (web_frame->IsWebLocalFrame())
      web_frame->ToWebLocalFrame()->SetScrollOffset(prev_scroll_offset_);
  }
}
