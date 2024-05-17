void Layer::InsertChild(scoped_refptr<Layer> child, size_t index) {
  DCHECK(IsPropertyChangeAllowed());
  child->RemoveFromParent();
  child->SetParent(this);
  child->stacking_order_changed_ = true;

  index = std::min(index, children_.size());
  children_.insert(children_.begin() + index, child);
  SetNeedsFullTreeSync();
}
