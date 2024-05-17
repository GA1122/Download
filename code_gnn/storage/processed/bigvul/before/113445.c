void InputMethodIBus::OnCommitText(
    IBusInputContext* context, IBusText* text) {
  DCHECK_EQ(context_, context);
  if (suppress_next_result_ || !text)
    return;

  if (!GetTextInputClient())
    return;

  const string16 utf16_text = ibus_client_->ExtractCommitText(text);
  if (utf16_text.empty())
    return;

  result_text_.append(utf16_text);

  if (pending_key_events_.empty() && !IsTextInputTypeNone()) {
    SendFakeProcessKeyEvent(true);
    GetTextInputClient()->InsertText(utf16_text);
    SendFakeProcessKeyEvent(false);
    result_text_.clear();
  }
}
