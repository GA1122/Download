void LayerTreeHostImpl::MouseDown() {
  ScrollbarAnimationController* animation_controller =
      ScrollbarAnimationControllerForId(scroll_layer_id_mouse_currently_over_);
  if (animation_controller) {
    animation_controller->DidMouseDown();
    scroll_layer_id_mouse_currently_captured_ =
        scroll_layer_id_mouse_currently_over_;
  }
}
