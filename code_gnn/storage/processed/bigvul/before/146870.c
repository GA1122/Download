void Document::UpdateViewportDescription() {
  if (GetFrame() && GetFrame()->IsMainFrame()) {
    GetPage()->GetChromeClient().DispatchViewportPropertiesDidChange(
        GetViewportDescription());
  }
}
