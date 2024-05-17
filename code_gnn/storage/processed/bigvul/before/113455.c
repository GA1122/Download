void InputMethodIBus::OnUpdatePreeditText(IBusInputContext* context,
                                          IBusText* text,
                                          guint cursor_pos,
                                          gboolean visible) {
  DCHECK_EQ(context_, context);
  if (suppress_next_result_ || IsTextInputTypeNone())
    return;

  if (!visible) {
    OnHidePreeditText(context);
    return;
  }

  ibus_client_->ExtractCompositionText(text, cursor_pos, &composition_);

  composition_changed_ = true;

  if (composition_.text.length())
    composing_text_ = true;

  if (pending_key_events_.empty()) {
    SendFakeProcessKeyEvent(true);
    GetTextInputClient()->SetCompositionText(composition_);
    SendFakeProcessKeyEvent(false);
    composition_changed_ = false;
    composition_.Clear();
  }
}
