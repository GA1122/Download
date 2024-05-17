 void InputMethodLinuxX11::CancelComposition(const TextInputClient* client) {
  if (!IsTextInputClientFocused(client))
    return;

  input_method_context_->Reset();
  input_method_context_->OnTextInputTypeChanged(client->GetTextInputType());
 }
