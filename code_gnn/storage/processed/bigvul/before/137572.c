void PrepareFrameAndViewForPrint::RestoreSize() {
  if (frame()) {
    blink::WebView* web_view = frame_.GetFrame()->view();
    web_view->resize(prev_view_size_);
    if (blink::WebFrame* web_frame = web_view->mainFrame())
      web_frame->setScrollOffset(prev_scroll_offset_);
  }
}
