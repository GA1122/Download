void LayerTreeHostImpl::RegisterScrollbarAnimationController(
    int scroll_layer_id) {
  if (settings().scrollbar_animator == LayerTreeSettings::NO_ANIMATOR)
    return;
  if (ScrollbarAnimationControllerForId(scroll_layer_id))
    return;
  scrollbar_animation_controllers_[scroll_layer_id] =
      active_tree_->CreateScrollbarAnimationController(scroll_layer_id);
}
