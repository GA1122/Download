void CustomButton::GetAccessibleState(ui::AXViewState* state) {
  Button::GetAccessibleState(state);
  switch (state_) {
    case STATE_HOVERED:
      state->AddStateFlag(ui::AX_STATE_HOVERED);
      break;
    case STATE_PRESSED:
      state->AddStateFlag(ui::AX_STATE_PRESSED);
      break;
    case STATE_DISABLED:
      state->AddStateFlag(ui::AX_STATE_DISABLED);
      break;
    case STATE_NORMAL:
    case STATE_COUNT:
      break;
  }
}
