void Document::UpdateStyleAndLayoutTreeIgnorePendingStylesheets() {
  if (RuntimeEnabledFeatures::CSSInBodyDoesNotBlockPaintEnabled()) {
    UpdateStyleAndLayoutTree();
    return;
  }
  if (Lifecycle().LifecyclePostponed())
    return;
  CHECK(Lifecycle().StateAllowsTreeMutations());
  StyleEngine::IgnoringPendingStylesheet ignoring(GetStyleEngine());

  if (!HaveRenderBlockingResourcesLoaded()) {
    HTMLElement* body_element = body();
    if (body_element && !body_element->GetLayoutObject() &&
        pending_sheet_layout_ == kNoLayoutWithPendingSheets) {
      pending_sheet_layout_ = kDidLayoutWithPendingSheets;
      GetStyleEngine().MarkAllTreeScopesDirty();
    }
    if (has_nodes_with_placeholder_style_) {
      SetNeedsStyleRecalc(kSubtreeStyleChange,
                          StyleChangeReasonForTracing::Create(
                              style_change_reason::kCleanupPlaceholderStyles));
    }
  }
  UpdateStyleAndLayoutTree();
}
