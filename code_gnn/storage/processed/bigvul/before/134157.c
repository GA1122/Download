 void InputMethodTSF::CancelComposition(const TextInputClient* client) {
  if (IsTextInputClientFocused(client) && IsWindowFocused(client))
    ui::TSFBridge::GetInstance()->CancelComposition();
}
