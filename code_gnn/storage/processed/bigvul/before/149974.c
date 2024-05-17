void LayerTreeHostImpl::MouseLeave() {
  for (auto& pair : scrollbar_animation_controllers_)
    pair.second->DidMouseLeave();
  scroll_layer_id_mouse_currently_over_ = Layer::INVALID_ID;
}
