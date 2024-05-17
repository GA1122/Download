bool Layer::IsPropertyChangeAllowed() const {
  if (!layer_tree_host_)
    return true;

  if (!layer_tree_host_->settings().strict_layer_property_change_checking)
    return true;

  return !layer_tree_host_->in_paint_layer_contents();
}
