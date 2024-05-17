void Layer::SetNeedsFilterContextIfNeeded() {
  if (!layer_tree_host_)
    return;

  if (!filters_.IsEmpty() || !background_filters_.IsEmpty() ||
      !uses_default_blend_mode())
    layer_tree_host_->set_needs_filter_context();
}
