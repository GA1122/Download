void Layer::SetNonFastScrollableRegion(const Region& region) {
  DCHECK(IsPropertyChangeAllowed());
  if (non_fast_scrollable_region_ == region)
    return;
  non_fast_scrollable_region_ = region;
  SetNeedsCommit();
}
