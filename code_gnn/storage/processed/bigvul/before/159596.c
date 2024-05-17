void Document::UpdateStyle() {
  DCHECK(!View()->ShouldThrottleRendering());
  TRACE_EVENT_BEGIN0("blink,blink_style", "Document::updateStyle");
  RUNTIME_CALL_TIMER_SCOPE(V8PerIsolateData::MainThreadIsolate(),
                           RuntimeCallStats::CounterId::kUpdateStyle);
  double start_time = MonotonicallyIncreasingTime();

  unsigned initial_element_count = GetStyleEngine().StyleForElementCount();

  lifecycle_.AdvanceTo(DocumentLifecycle::kInStyleRecalc);

  StyleRecalcChange change = kNoChange;
  if (GetStyleChangeType() >= kSubtreeStyleChange)
    change = kForce;

  NthIndexCache nth_index_cache(*this);


  if (change == kForce) {
    has_nodes_with_placeholder_style_ = false;
    scoped_refptr<ComputedStyle> viewport_style =
        StyleResolver::StyleForViewport(*this);
    StyleRecalcChange local_change = ComputedStyle::StylePropagationDiff(
        viewport_style.get(), GetLayoutViewItem().Style());
    if (local_change != kNoChange)
      GetLayoutViewItem().SetStyle(std::move(viewport_style));
  }

  ClearNeedsStyleRecalc();
  ClearNeedsReattachLayoutTree();

  StyleResolver& resolver = EnsureStyleResolver();

  bool should_record_stats;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED("blink,blink_style", &should_record_stats);
  GetStyleEngine().SetStatsEnabled(should_record_stats);

  if (Element* document_element = documentElement()) {
    if (document_element->ShouldCallRecalcStyle(change)) {
      TRACE_EVENT0("blink,blink_style", "Document::recalcStyle");
      Element* viewport_defining = ViewportDefiningElement();
      document_element->RecalcStyle(change);
      if (viewport_defining != ViewportDefiningElement())
        ViewportDefiningElementDidChange();
    }
    GetStyleEngine().MarkForWhitespaceReattachment();
    PropagateStyleToViewport();
    if (document_element->NeedsReattachLayoutTree() ||
        document_element->ChildNeedsReattachLayoutTree()) {
      TRACE_EVENT0("blink,blink_style", "Document::rebuildLayoutTree");
      WhitespaceAttacher whitespace_attacher;
      document_element->RebuildLayoutTree(whitespace_attacher);
    }
  }
  GetStyleEngine().ClearWhitespaceReattachSet();

  View()->UpdateCountersAfterStyleChange();
  View()->RecalcOverflowAfterStyleChange();

  ClearChildNeedsStyleRecalc();
  ClearChildNeedsReattachLayoutTree();

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

  double update_duration_seconds = MonotonicallyIncreasingTime() - start_time;
  DEFINE_STATIC_LOCAL(CustomCountHistogram, update_histogram,
                      ("Style.UpdateTime", 0, 10000000, 50));
  update_histogram.Count(update_duration_seconds * 1000 * 1000);
  CSSTiming::From(*this).RecordUpdateDuration(update_duration_seconds);
}
