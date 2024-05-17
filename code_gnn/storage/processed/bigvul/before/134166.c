void InputMethodTSF::OnTextInputTypeChanged(const TextInputClient* client) {
  if (IsTextInputClientFocused(client) && IsWindowFocused(client)) {
    ui::TSFBridge::GetInstance()->CancelComposition();
    ui::TSFBridge::GetInstance()->OnTextInputTypeChanged(client);
  }
  InputMethodWin::OnTextInputTypeChanged(client);
}
