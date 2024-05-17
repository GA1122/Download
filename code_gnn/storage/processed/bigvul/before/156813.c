void Document::LayoutUpdated() {
  DCHECK(GetFrame());
  DCHECK(View());

  View()->ScrollAndFocusFragmentAnchor();

  if (GetFrame() && View()) {
    GetFrame()->Loader().RestoreScrollPositionAndViewState();

    if (View()->NeedsLayout())
      View()->UpdateLayout();
  }

  if (GetFrame() && GetFrame()->IsMainFrame())
    GetFrame()->GetPage()->GetChromeClient().MainFrameLayoutUpdated();

  Markers().InvalidateRectsForAllTextMatchMarkers();

  if (IsRenderingReady() && body() && HaveRenderBlockingResourcesLoaded()) {
    if (document_timing_.FirstLayout().is_null())
      document_timing_.MarkFirstLayout();
  }
}
