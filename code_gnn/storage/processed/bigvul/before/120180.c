void Layer::SetOpacity(float opacity) {
  DCHECK(IsPropertyChangeAllowed());
  if (opacity_ == opacity)
    return;
  opacity_ = opacity;
  SetNeedsCommit();
}
