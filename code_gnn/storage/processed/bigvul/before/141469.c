void PaintLayerScrollableArea::PreventRelayoutScope::ResetRelayoutNeeded() {
  DCHECK_EQ(count_, 0);
  DCHECK(NeedsRelayoutList().IsEmpty());
  relayout_needed_ = false;
}
