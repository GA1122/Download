void Layer::SetTransform(const gfx::Transform& transform) {
  DCHECK(IsPropertyChangeAllowed());
  if (transform_ == transform)
    return;
  transform_ = transform;
  SetNeedsCommit();
}
