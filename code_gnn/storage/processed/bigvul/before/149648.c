void ScrollAnchor::Adjust() {
  if (!queued_)
    return;
  queued_ = false;
  DCHECK(scroller_);
  if (!anchor_object_)
    return;
  IntSize adjustment = ComputeAdjustment();
  if (adjustment.IsZero())
    return;

  if (scroll_anchor_disabling_style_changed_) {
    ClearSelf();

    DEFINE_STATIC_LOCAL(EnumerationHistogram, suppressed_by_sanaclap_histogram,
                        ("Layout.ScrollAnchor.SuppressedBySanaclap", 2));
    suppressed_by_sanaclap_histogram.Count(1);

    return;
  }

  scroller_->SetScrollOffset(
      scroller_->GetScrollOffset() + FloatSize(adjustment), kAnchoringScroll);

  DEFINE_STATIC_LOCAL(EnumerationHistogram, adjusted_offset_histogram,
                      ("Layout.ScrollAnchor.AdjustedScrollOffset", 2));
  adjusted_offset_histogram.Count(1);
  UseCounter::Count(ScrollerLayoutBox(scroller_)->GetDocument(),
                    WebFeature::kScrollAnchored);
}
