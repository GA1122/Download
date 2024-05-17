void PrintRenderFrameHelper::OnClosePrintPreviewDialog() {
  print_preview_context_.source_frame()->DispatchAfterPrintEvent();
}
