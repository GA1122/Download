void Layer::RemoveAllChildren() {
  DCHECK(IsPropertyChangeAllowed());
  while (children_.size()) {
    Layer* layer = children_[0].get();
    DCHECK_EQ(this, layer->parent());
    layer->RemoveFromParent();
  }
}
