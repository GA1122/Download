void MockInputMethod::CancelComposition(const ui::TextInputClient* client) {
  if (IsTextInputClientFocused(client)) {
    cancel_composition_called_ = true;
    ClearComposition();
  }
}
