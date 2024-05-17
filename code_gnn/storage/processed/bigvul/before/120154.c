void Layer::SetBounds(const gfx::Size& size) {
  DCHECK(IsPropertyChangeAllowed());
  if (bounds() == size)
    return;

  bounds_ = size;
  SetNeedsCommit();
}
