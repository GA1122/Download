void Layer::SetIsRootForIsolatedGroup(bool root) {
  DCHECK(IsPropertyChangeAllowed());
  if (is_root_for_isolated_group_ == root)
    return;
  is_root_for_isolated_group_ = root;
  SetNeedsCommit();
}
