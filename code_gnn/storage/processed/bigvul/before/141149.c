void Document::LayoutUpdated() {
  DCHECK(GetFrame());
  DCHECK(View());

  View()->InvokeFragmentAnchor();
  GetFrame()->Loader().RestoreScrollPositionAndViewState();

  if (GetFrame() && GetFrame()->IsMainFrame())
    GetFrame()->GetPage()->GetChromeClient().MainFrameLayoutUpdated();

  Markers().InvalidateRectsForAllTextMatchMarkers();

  if (HaveRenderBlockingResourcesLoaded()) {
    if (document_timing_.FirstLayout().is_null())
      document_timing_.MarkFirstLayout();
  }
}
