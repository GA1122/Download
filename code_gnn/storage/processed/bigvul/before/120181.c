void Layer::SetParent(Layer* layer) {
  DCHECK(!layer || !layer->HasAncestor(this));

  if (parent_should_know_need_push_properties()) {
    if (parent_)
      parent_->RemoveDependentNeedsPushProperties();
    if (layer)
      layer->AddDependentNeedsPushProperties();
  }

  parent_ = layer;
  SetLayerTreeHost(parent_ ? parent_->layer_tree_host() : NULL);

  if (!layer_tree_host_)
    return;
  const LayerTreeSettings& settings = layer_tree_host_->settings();
  if (!settings.layer_transforms_should_scale_layer_contents)
    return;

  reset_raster_scale_to_unknown();
  if (mask_layer_.get())
    mask_layer_->reset_raster_scale_to_unknown();
  if (replica_layer_.get() && replica_layer_->mask_layer_.get())
    replica_layer_->mask_layer_->reset_raster_scale_to_unknown();
}
