IntSize ScrollAnchor::ComputeAdjustment() const {
  IntSize delta = RoundedIntSize(ComputeRelativeOffset(anchor_object_,
                                                       scroller_, corner_)) -
                  RoundedIntSize(saved_relative_offset_);

  if (ScrollerLayoutBox(scroller_)->IsHorizontalWritingMode())
    delta.SetWidth(0);
  else
    delta.SetHeight(0);
  return delta;
}
