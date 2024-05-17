void PDFiumEngine::InvalidateAllPages() {
  CancelPaints();
  StopFind();
  LoadPageInfo(true);
  client_->Invalidate(pp::Rect(plugin_size_));
}
