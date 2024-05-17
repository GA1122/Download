void PDFiumEngine::PluginSizeUpdated(const pp::Size& size) {
  CancelPaints();

  plugin_size_ = size;
  CalculateVisiblePages();
}
