void Layer::SetLayerTreeHost(LayerTreeHost* host) {
  if (layer_tree_host_ == host)
    return;

  layer_tree_host_ = host;

  SetNeedsPushProperties();

  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->SetLayerTreeHost(host);

  if (mask_layer_.get())
    mask_layer_->SetLayerTreeHost(host);
  if (replica_layer_.get())
    replica_layer_->SetLayerTreeHost(host);

  if (host) {
    layer_animation_controller_->SetAnimationRegistrar(
        host->animation_registrar());

    if (host->settings().layer_transforms_should_scale_layer_contents)
      reset_raster_scale_to_unknown();
  }

  if (host && layer_animation_controller_->has_any_animation())
    host->SetNeedsCommit();
  SetNeedsFilterContextIfNeeded();
}
