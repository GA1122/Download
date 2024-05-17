void PrintWebViewHelper::OnFramePreparedForPrintPages() {
  PrintPages();
  FinishFramePrinting();
}
