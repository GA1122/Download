void MockInputMethod::OnTextInputTypeChanged(
    const ui::TextInputClient* client) {
  if (IsTextInputClientFocused(client))
    text_input_type_changed_ = true;
  InputMethodBase::OnTextInputTypeChanged(client);
}
