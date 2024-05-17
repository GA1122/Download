void ScrollAnchor::FindAnchor() {
  TRACE_EVENT0("blink", "ScrollAnchor::findAnchor");
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER("Layout.ScrollAnchor.TimeToFindAnchor");
  FindAnchorRecursive(ScrollerLayoutBox(scroller_));
  if (anchor_object_) {
    anchor_object_->SetIsScrollAnchorObject();
    saved_relative_offset_ =
        ComputeRelativeOffset(anchor_object_, scroller_, corner_);
  }
}
