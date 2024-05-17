bool InputHandlerProxy::GetSnapFlingInfo(
    const gfx::Vector2dF& natural_displacement,
    gfx::Vector2dF* initial_offset,
    gfx::Vector2dF* target_offset) const {
  return input_handler_->GetSnapFlingInfo(natural_displacement, initial_offset,
                                          target_offset);
}
