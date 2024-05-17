void Layer::SetAnchorPoint(const gfx::PointF& anchor_point) {
  DCHECK(IsPropertyChangeAllowed());
  if (anchor_point_ == anchor_point)
    return;
  anchor_point_ = anchor_point;
  SetNeedsCommit();
}
