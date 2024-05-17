bool Instance::HasScriptableMethod(const pp::Var& method, pp::Var* exception) {
  std::string method_str = method.AsString();
  return (method_str == kJSAccessibility ||
          method_str == kJSDocumentLoadComplete ||
          method_str == kJSGetHeight ||
          method_str == kJSGetHorizontalScrollbarThickness ||
          method_str == kJSGetPageLocationNormalized ||
          method_str == kJSGetVerticalScrollbarThickness ||
          method_str == kJSGetWidth ||
          method_str == kJSGetZoomLevel ||
          method_str == kJSGoToPage ||
          method_str == kJSGrayscale ||
          method_str == kJSLoadPreviewPage ||
          method_str == kJSOnLoad ||
          method_str == kJSOnPluginSizeChanged ||
          method_str == kJSOnScroll ||
          method_str == kJSPageXOffset ||
          method_str == kJSPageYOffset ||
          method_str == kJSPrintPreviewPageCount ||
          method_str == kJSReload ||
          method_str == kJSRemovePrintButton ||
          method_str == kJSResetPrintPreviewUrl ||
          method_str == kJSSendKeyEvent ||
          method_str == kJSSetPageNumbers ||
          method_str == kJSSetPageXOffset ||
          method_str == kJSSetPageYOffset ||
          method_str == kJSSetZoomLevel ||
          method_str == kJSZoomFitToHeight ||
          method_str == kJSZoomFitToWidth ||
          method_str == kJSZoomIn ||
          method_str == kJSZoomOut);
}
