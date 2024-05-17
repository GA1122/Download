void Layer::SetAnchorPointZ(float anchor_point_z) {
  DCHECK(IsPropertyChangeAllowed());
  if (anchor_point_z_ == anchor_point_z)
    return;
  anchor_point_z_ = anchor_point_z;
  SetNeedsCommit();
}
