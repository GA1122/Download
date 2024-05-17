bool Compositor::GetScrollOffsetForLayer(cc::ElementId element_id,
                                         gfx::ScrollOffset* offset) const {
  auto input_handler = host_->GetInputHandler();
  return input_handler &&
         input_handler->GetScrollOffsetForLayer(element_id, offset);
}
