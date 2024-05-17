void Layer::SetIs3dSorted(bool sorted) {
  DCHECK(IsPropertyChangeAllowed());
  if (is_3d_sorted_ == sorted)
    return;
  is_3d_sorted_ = sorted;
  SetNeedsCommit();
}
