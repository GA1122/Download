Layer::~Layer() {
  DCHECK(!parent());
  DCHECK(!layer_tree_host());

  layer_animation_controller_->RemoveValueObserver(this);
  layer_animation_controller_->remove_value_provider(this);

  RemoveAllChildren();
  if (mask_layer_.get()) {
    DCHECK_EQ(this, mask_layer_->parent());
    mask_layer_->RemoveFromParent();
  }
  if (replica_layer_.get()) {
    DCHECK_EQ(this, replica_layer_->parent());
    replica_layer_->RemoveFromParent();
  }

  RemoveFromScrollTree();
  RemoveFromClipTree();
}
