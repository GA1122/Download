void Document::StyleResolverMayHaveChanged() {
  if (HasNodesWithPlaceholderStyle()) {
    SetNeedsStyleRecalc(kSubtreeStyleChange,
                        StyleChangeReasonForTracing::Create(
                            StyleChangeReason::kCleanupPlaceholderStyles));
  }

  if (DidLayoutWithPendingStylesheets() &&
      HaveRenderBlockingResourcesLoaded()) {
    pending_sheet_layout_ = kIgnoreLayoutWithPendingSheets;

    DCHECK(GetLayoutView() || ImportsController());
    if (GetLayoutView())
      GetLayoutView()->InvalidatePaintForViewAndCompositedLayers();
  }
}
