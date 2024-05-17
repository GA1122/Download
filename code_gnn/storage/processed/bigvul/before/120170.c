void Layer::SetMaskLayer(Layer* mask_layer) {
  DCHECK(IsPropertyChangeAllowed());
  if (mask_layer_.get() == mask_layer)
    return;
  if (mask_layer_.get()) {
    DCHECK_EQ(this, mask_layer_->parent());
    mask_layer_->RemoveFromParent();
  }
  mask_layer_ = mask_layer;
  if (mask_layer_.get()) {
    DCHECK(!mask_layer_->parent());
    mask_layer_->RemoveFromParent();
    mask_layer_->SetParent(this);
    mask_layer_->SetIsMask(true);
  }
  SetNeedsFullTreeSync();
}
