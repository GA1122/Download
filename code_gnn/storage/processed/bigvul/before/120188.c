void Layer::SetShouldFlattenTransform(bool should_flatten) {
  DCHECK(IsPropertyChangeAllowed());
  if (should_flatten_transform_ == should_flatten)
    return;
  should_flatten_transform_ = should_flatten;
  SetNeedsCommit();
}
