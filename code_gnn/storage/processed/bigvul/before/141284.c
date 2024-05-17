void Document::UpdateStyle() {
  DCHECK(!View()->ShouldThrottleRendering());
  TRACE_EVENT_BEGIN0("blink,blink_style", "Document::updateStyle");
  RUNTIME_CALL_TIMER_SCOPE(V8PerIsolateData::MainThreadIsolate(),
                           RuntimeCallStats::CounterId::kUpdateStyle);

  unsigned initial_element_count = GetStyleEngine().StyleForElementCount();

  lifecycle_.AdvanceTo(DocumentLifecycle::kInStyleRecalc);

  DCHECK(!ChildNeedsReattachLayoutTree());
  DCHECK(!NeedsReattachLayoutTree());

  NthIndexCache nth_index_cache(*this);

  StyleRecalcChange change;
  if (GetStyleChangeType() == kSubtreeStyleChange) {
    change = change.ForceRecalcDescendants();

    scoped_refptr<ComputedStyle> viewport_style =
        StyleResolver::StyleForViewport(*this);
    if (ComputedStyle::ComputeDifference(viewport_style.get(),
                                         GetLayoutView()->Style()) !=
        ComputedStyle::Difference::kEqual) {
      GetLayoutView()->SetStyle(std::move(viewport_style));
    }
  }

  ClearNeedsStyleRecalc();

  StyleResolver& resolver = EnsureStyleResolver();

  bool should_record_stats;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED("blink,blink_style", &should_record_stats);
  GetStyleEngine().SetStatsEnabled(should_record_stats);

  if (Element* document_element = documentElement()) {
    if (change.TraverseChild(*document_element)) {
      TRACE_EVENT0("blink,blink_style", "Document::recalcStyle");
      SCOPED_BLINK_UMA_HISTOGRAM_TIMER_HIGHRES("Style.RecalcTime");
      Element* viewport_defining = ViewportDefiningElement();
      GetStyleEngine().RecalcStyle(change);
      if (viewport_defining != ViewportDefiningElement())
        ViewportDefiningElementDidChange();
    }
    GetStyleEngine().MarkForWhitespaceReattachment();
    if (document_element->NeedsReattachLayoutTree() ||
        document_element->ChildNeedsReattachLayoutTree()) {
      TRACE_EVENT0("blink,blink_style", "Document::rebuildLayoutTree");
      SCOPED_BLINK_UMA_HISTOGRAM_TIMER_HIGHRES("Style.RebuildLayoutTreeTime");
      ReattachLegacyLayoutObjectList legacy_layout_objects(*this);
      GetStyleEngine().RebuildLayoutTree();
      legacy_layout_objects.ForceLegacyLayoutIfNeeded();
    }
  }
  GetStyleEngine().ClearWhitespaceReattachSet();
  ClearChildNeedsStyleRecalc();
  ClearChildNeedsReattachLayoutTree();

  PropagateStyleToViewport();
  View()->UpdateCountersAfterStyleChange();
  GetLayoutView()->RecalcLayoutOverflow();

  DCHECK(!NeedsStyleRecalc());
  DCHECK(!ChildNeedsStyleRecalc());
  DCHECK(!NeedsReattachLayoutTree());
  DCHECK(!ChildNeedsReattachLayoutTree());
  DCHECK(InStyleRecalc());
  DCHECK_EQ(GetStyleResolver(), &resolver);
  lifecycle_.AdvanceTo(DocumentLifecycle::kStyleClean);
  if (should_record_stats) {
    TRACE_EVENT_END2(
        "blink,blink_style", "Document::updateStyle", "resolverAccessCount",
        GetStyleEngine().StyleForElementCount() - initial_element_count,
        "counters", GetStyleEngine().Stats()->ToTracedValue());
  } else {
    TRACE_EVENT_END1(
        "blink,blink_style", "Document::updateStyle", "resolverAccessCount",
        GetStyleEngine().StyleForElementCount() - initial_element_count);
  }
}
