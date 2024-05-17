void InputMethodTSF::OnDidChangeFocusedClient(TextInputClient* focused_before,
                                              TextInputClient* focused) {
  if (IsWindowFocused(focused) && IsTextInputClientFocused(focused)) {
    ui::TSFBridge::GetInstance()->SetFocusedClient(
        GetAttachedWindowHandle(focused), focused);

    OnTextInputTypeChanged(focused);

    OnCaretBoundsChanged(focused);
  }
}
