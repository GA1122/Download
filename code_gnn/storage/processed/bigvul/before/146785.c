void Document::LayoutUpdated() {
  if (GetFrame() && GetFrame()->IsMainFrame())
    GetFrame()->GetPage()->GetChromeClient().LayoutUpdated();

  Markers().InvalidateRectsForAllTextMatchMarkers();

  if (IsRenderingReady() && body() &&
      !GetStyleEngine().HasPendingScriptBlockingSheets()) {
    if (!document_timing_.FirstLayout())
      document_timing_.MarkFirstLayout();
  }

  root_scroller_controller_->DidUpdateLayout();
}
