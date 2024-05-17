void Document::StyleResolverMayHaveChanged() {
  if (HasNodesWithPlaceholderStyle()) {
    SetNeedsStyleRecalc(kSubtreeStyleChange,
                        StyleChangeReasonForTracing::Create(
                            StyleChangeReason::kCleanupPlaceholderStyles));
  }

  if (DidLayoutWithPendingStylesheets() &&
      !GetStyleEngine().HasPendingScriptBlockingSheets()) {
    pending_sheet_layout_ = kIgnoreLayoutWithPendingSheets;

    DCHECK(!GetLayoutViewItem().IsNull() || ImportsController());
    if (!GetLayoutViewItem().IsNull())
      GetLayoutViewItem().InvalidatePaintForViewAndCompositedLayers();
  }
}
