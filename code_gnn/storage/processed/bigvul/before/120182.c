void Layer::SetPosition(const gfx::PointF& position) {
  DCHECK(IsPropertyChangeAllowed());
  if (position_ == position)
    return;
  position_ = position;
  SetNeedsCommit();
}
