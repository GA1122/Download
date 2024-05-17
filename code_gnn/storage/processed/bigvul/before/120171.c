void Layer::SetMasksToBounds(bool masks_to_bounds) {
  DCHECK(IsPropertyChangeAllowed());
  if (masks_to_bounds_ == masks_to_bounds)
    return;
  masks_to_bounds_ = masks_to_bounds;
  SetNeedsCommit();
}
