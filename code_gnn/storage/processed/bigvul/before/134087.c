void InputMethodBase::SetFocusedTextInputClient(TextInputClient* client) {
  if (is_sticky_text_input_client_)
    return;
  SetFocusedTextInputClientInternal(client);
}
