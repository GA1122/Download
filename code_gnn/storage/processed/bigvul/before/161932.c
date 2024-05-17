void PrintRenderFrameHelper::OnFramePreparedForPrintPages() {
  PrintPages();
  FinishFramePrinting();
}
