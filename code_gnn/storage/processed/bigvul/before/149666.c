bool ScrollAnchor::RestoreAnchor(const SerializedAnchor& serialized_anchor) {
  if (!scroller_ || !serialized_anchor.IsValid()) {
    return false;
  }

  SCOPED_BLINK_UMA_HISTOGRAM_TIMER("Layout.ScrollAnchor.TimeToRestoreAnchor");
  DEFINE_STATIC_LOCAL(EnumerationHistogram, restoration_status_histogram,
                      ("Layout.ScrollAnchor.RestorationStatus", kStatusCount));

  if (anchor_object_ && serialized_anchor.selector == saved_selector_) {
    return true;
  }

  if (anchor_object_) {
    return false;
  }

  Document* document = &(ScrollerLayoutBox(scroller_)->GetDocument());

  DummyExceptionStateForTesting exception_state;
  StaticElementList* found_elements = document->QuerySelectorAll(
      AtomicString(serialized_anchor.selector), exception_state);

  if (exception_state.HadException()) {
    restoration_status_histogram.Count(kFailedBadSelector);
    return false;
  }

  if (found_elements->length() < 1) {
    restoration_status_histogram.Count(kFailedNoMatches);
    return false;
  }

  for (unsigned index = 0; index < found_elements->length(); index++) {
    Element* anchor_element = found_elements->item(index);
    LayoutObject* anchor_object = anchor_element->GetLayoutObject();

    if (!anchor_object) {
      continue;
    }

    ScrollOffset current_offset = scroller_->GetScrollOffset();
    FloatPoint desired_point =
        anchor_object->AbsoluteBoundingBoxFloatRect().Location() +
        current_offset;
    ScrollOffset desired_offset =
        ScrollOffset(desired_point.X(), desired_point.Y());
    ScrollOffset delta =
        ScrollOffset(RoundedIntSize(serialized_anchor.relative_offset));
    desired_offset -= delta;
    scroller_->SetScrollOffset(desired_offset, kAnchoringScroll);
    FindAnchor();

    if (!anchor_object_) {
      scroller_->SetScrollOffset(current_offset, kAnchoringScroll);
      continue;
    }

    saved_selector_ = serialized_anchor.selector;
    restoration_status_histogram.Count(kSuccess);

    return true;
  }

  restoration_status_histogram.Count(kFailedNoValidMatches);
  return false;
}
