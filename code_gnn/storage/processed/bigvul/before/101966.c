void PrintWebViewHelper::OnPrintForSystemDialog() {
  WebFrame* frame = print_preview_context_.frame();
  if (!frame) {
    NOTREACHED();
    return;
  }

  WebNode* node = print_preview_context_.node();
  if (!node) {
    Print(frame, NULL);
  } else {
    WebNode duplicate_node(*node);
    Print(frame, &duplicate_node);
  }
}
