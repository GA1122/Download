void PrintRenderFrameHelper::OnInitiatePrintPreview(bool has_selection) {
  if (ipc_nesting_level_ > 1)
    return;

  blink::WebLocalFrame* frame = render_frame()->GetWebFrame();

  auto plugin = delegate_->GetPdfElement(frame);
  if (!plugin.IsNull()) {
    PrintNode(plugin);
    return;
  }
  print_preview_context_.InitWithFrame(frame);
  RequestPrintPreview(has_selection
                          ? PRINT_PREVIEW_USER_INITIATED_SELECTION
                          : PRINT_PREVIEW_USER_INITIATED_ENTIRE_FRAME);
}
