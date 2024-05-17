const SerializedAnchor ScrollAnchor::GetSerializedAnchor() {
  if (!saved_selector_.IsEmpty()) {
    DCHECK(anchor_object_);
    return SerializedAnchor(
        saved_selector_,
        ComputeRelativeOffset(anchor_object_, scroller_, corner_));
  }

  if (!anchor_object_) {
    FindAnchor();
    if (!anchor_object_)
      return SerializedAnchor();
  }

  DCHECK(anchor_object_->GetNode());
  SerializedAnchor new_anchor(
      ComputeUniqueSelector(anchor_object_->GetNode()),
      ComputeRelativeOffset(anchor_object_, scroller_, corner_));
  if (new_anchor.IsValid()) {
    saved_selector_ = new_anchor.selector;
  }

  return new_anchor;
}
