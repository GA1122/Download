void RenderViewImpl::draggableRegionsChanged() {
  FOR_EACH_OBSERVER(
      RenderViewObserver,
      observers_,
      DraggableRegionsChanged(webview()->mainFrame()));
}
