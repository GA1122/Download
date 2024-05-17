bool Compositor::ScrollLayerTo(cc::ElementId element_id,
                               const gfx::ScrollOffset& offset) {
  auto input_handler = host_->GetInputHandler();
  return input_handler && input_handler->ScrollLayerTo(element_id, offset);
}
