void InputMethodLinuxX11::OnTextInputTypeChanged(
    const TextInputClient* client) {
  if (IsTextInputClientFocused(client)) {
    input_method_context_->Reset();
    input_method_context_->OnTextInputTypeChanged(client->GetTextInputType());
  }
   InputMethodBase::OnTextInputTypeChanged(client);
 }
