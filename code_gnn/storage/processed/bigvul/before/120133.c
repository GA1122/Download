void Layer::RemoveChildOrDependent(Layer* child) {
  if (mask_layer_.get() == child) {
    mask_layer_->SetParent(NULL);
    mask_layer_ = NULL;
    SetNeedsFullTreeSync();
    return;
  }
  if (replica_layer_.get() == child) {
    replica_layer_->SetParent(NULL);
    replica_layer_ = NULL;
    SetNeedsFullTreeSync();
    return;
  }

  for (LayerList::iterator iter = children_.begin();
       iter != children_.end();
       ++iter) {
    if (iter->get() != child)
      continue;

    child->SetParent(NULL);
    children_.erase(iter);
    SetNeedsFullTreeSync();
    return;
  }
}
