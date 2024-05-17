void Layer::RemoveFromClipTree() {
  if (clip_children_.get()) {
    for (std::set<Layer*>::iterator it = clip_children_->begin();
        it != clip_children_->end(); ++it)
      (*it)->clip_parent_ = NULL;
  }

  if (clip_parent_)
    clip_parent_->RemoveClipChild(this);

  clip_parent_ = NULL;
}
