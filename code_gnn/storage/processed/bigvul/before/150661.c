gfx::Vector2dF InputHandlerProxy::ScrollByForSnapFling(
    const gfx::Vector2dF& delta) {
  cc::ScrollState scroll_state = CreateScrollStateForInertialUpdate(delta);
  cc::InputHandlerScrollResult scroll_result =
      input_handler_->ScrollBy(&scroll_state);
  return scroll_result.current_visual_offset;
}
