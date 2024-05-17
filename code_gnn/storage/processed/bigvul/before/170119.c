void InputConnectionImpl::UpdateTextInputState(
    bool is_input_state_update_requested) {
  if (state_update_timer_.IsRunning()) {
    is_input_state_update_requested = true;
  }
  state_update_timer_.Stop();
  imm_bridge_->SendUpdateTextInputState(
      GetTextInputState(is_input_state_update_requested));
}
