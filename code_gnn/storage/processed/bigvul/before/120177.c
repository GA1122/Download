void Layer::SetNeedsUpdate() {
  if (layer_tree_host_ && !ignore_set_needs_commit_)
    layer_tree_host_->SetNeedsUpdateLayers();
}
