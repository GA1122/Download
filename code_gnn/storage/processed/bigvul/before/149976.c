void LayerTreeHostImpl::MouseUp() {
  if (scroll_layer_id_mouse_currently_captured_ != Layer::INVALID_ID) {
    ScrollbarAnimationController* animation_controller =
        ScrollbarAnimationControllerForId(
            scroll_layer_id_mouse_currently_captured_);

    scroll_layer_id_mouse_currently_captured_ = Layer::INVALID_ID;

    if (animation_controller)
      animation_controller->DidMouseUp();
  }
}
