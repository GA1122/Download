void Layer::SetClipParent(Layer* ancestor) {
  DCHECK(IsPropertyChangeAllowed());
  if (clip_parent_ == ancestor)
    return;

  if (clip_parent_)
    clip_parent_->RemoveClipChild(this);

  clip_parent_ = ancestor;

  if (clip_parent_)
    clip_parent_->AddClipChild(this);

  SetNeedsCommit();
}
