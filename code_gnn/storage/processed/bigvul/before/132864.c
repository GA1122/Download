PictureLayerImpl::~PictureLayerImpl() {
  if (twin_layer_)
    twin_layer_->twin_layer_ = nullptr;
  layer_tree_impl()->UnregisterPictureLayerImpl(this);
}
