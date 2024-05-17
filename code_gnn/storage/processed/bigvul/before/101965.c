void PrintWebViewHelper::OnInitiatePrintPreview() {
  DCHECK(is_preview_);
  WebFrame* frame;
  if (GetPrintFrame(&frame)) {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview();
  }
}
