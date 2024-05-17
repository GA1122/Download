void PrepareFrameAndViewForPrint::FinishPrinting() {
  blink::WebLocalFrame* frame = frame_.GetFrame();
  if (frame) {
    blink::WebView* web_view = frame->view();
    if (is_printing_started_) {
      is_printing_started_ = false;
      frame->printEnd();
      if (!owns_web_view_) {
        web_view->settings()->setShouldPrintBackgrounds(false);
        RestoreSize();
      }
    }
    if (owns_web_view_) {
      DCHECK(!frame->isLoading());
      owns_web_view_ = false;
      web_view->close();
    }
  }
  frame_.Reset(NULL);
  on_ready_.Reset();
}
