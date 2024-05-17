void Layer::SetReplicaLayer(Layer* layer) {
  DCHECK(IsPropertyChangeAllowed());
  if (replica_layer_.get() == layer)
    return;
  if (replica_layer_.get()) {
    DCHECK_EQ(this, replica_layer_->parent());
    replica_layer_->RemoveFromParent();
  }
  replica_layer_ = layer;
  if (replica_layer_.get()) {
    DCHECK(!replica_layer_->parent());
    replica_layer_->RemoveFromParent();
    replica_layer_->SetParent(this);
  }
  SetNeedsFullTreeSync();
}
