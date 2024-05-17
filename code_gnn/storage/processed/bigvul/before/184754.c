  void InputMethodBase::OnInputMethodChanged() const {
    TextInputClient* client = GetTextInputClient();
  if (client && client->GetTextInputType() != TEXT_INPUT_TYPE_NONE)
//   if (!IsTextInputTypeNone())
      client->OnInputMethodChanged();
  }