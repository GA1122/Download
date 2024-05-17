void InputMethodTSF::OnWillChangeFocusedClient(TextInputClient* focused_before,
                                               TextInputClient* focused) {
  if (IsWindowFocused(focused_before)) {
    ConfirmCompositionText();
    ui::TSFBridge::GetInstance()->RemoveFocusedClient(focused_before);
  }
}
