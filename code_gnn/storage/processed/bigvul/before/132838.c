PictureLayerImpl* PictureLayerImpl::GetRecycledTwinLayer() const {
  if (!twin_layer_ || twin_layer_->IsOnActiveOrPendingTree())
    return nullptr;
  return twin_layer_;
}
