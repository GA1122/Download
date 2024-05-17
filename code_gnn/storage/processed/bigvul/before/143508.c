bool CompositorImpl::HavePendingReadbacks() {
  return !readback_layer_tree_->children().empty();
}
