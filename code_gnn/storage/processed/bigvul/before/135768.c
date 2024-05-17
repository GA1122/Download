void InputMethodController::WillChangeFocus() {
  FinishComposingText(kKeepSelection);
}
