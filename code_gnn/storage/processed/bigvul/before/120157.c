void Layer::SetContentsOpaque(bool opaque) {
  DCHECK(IsPropertyChangeAllowed());
  if (contents_opaque_ == opaque)
    return;
  contents_opaque_ = opaque;
  SetNeedsCommit();
}
