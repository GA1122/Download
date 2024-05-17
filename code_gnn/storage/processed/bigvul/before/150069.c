void LayerTreeHostImpl::UnregisterScrollbarAnimationController(
    int scroll_layer_id) {
  scrollbar_animation_controllers_.erase(scroll_layer_id);
}
