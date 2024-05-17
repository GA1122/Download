void InputMethodIBus::OnHidePreeditText(IBusInputContext* context) {
  DCHECK_EQ(context_, context);
  if (composition_.text.empty() || IsTextInputTypeNone())
    return;

  composition_changed_ = true;
  composition_.Clear();

  if (pending_key_events_.empty()) {
    TextInputClient* client = GetTextInputClient();
    if (client && client->HasCompositionText())
      client->ClearCompositionText();
    composition_changed_ = false;
  }
}
