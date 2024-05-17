bool PrintWebViewHelper::RenderPagesForPrint(
    WebKit::WebFrame* frame,
    const WebKit::WebNode& node) {
  if (print_pages_params_->params.selection_only)
    return CopyAndPrint(frame);
  return PrintPages(frame, node);
}
