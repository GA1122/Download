void PrintWebViewHelper::OnInitiatePrintPreview() {
  DCHECK(is_preview_enabled_);
  WebKit::WebFrame* frame;
  if (GetPrintFrame(&frame)) {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview(PRINT_PREVIEW_USER_INITIATED_ENTIRE_FRAME);
  } else {
    CHECK(false);
  }
}
