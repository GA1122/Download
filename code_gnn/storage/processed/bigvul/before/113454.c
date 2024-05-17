void InputMethodIBus::OnTextInputTypeChanged(const TextInputClient* client) {
  if (context_ && IsTextInputClientFocused(client)) {
    ResetContext();
    UpdateContextFocusState();
  }
  InputMethodBase::OnTextInputTypeChanged(client);
}
