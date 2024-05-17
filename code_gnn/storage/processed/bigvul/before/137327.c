ui::EventDispatchDetails MockInputMethod::DispatchKeyEvent(ui::KeyEvent* key) {
#if defined(OS_MACOSX)
  if (key->is_char())
    return DispatchKeyEventPostIME(key);
#endif

  if (key->is_char() && key->HasNativeEvent()) {
    key->SetHandled();
    return ui::EventDispatchDetails();
  }

  ui::EventDispatchDetails dispatch_details;

  bool handled = !IsTextInputTypeNone() && HasComposition();
  ClearStates();
  if (handled) {
    DCHECK(!key->is_char());
    ui::KeyEvent mock_key(ui::ET_KEY_PRESSED,
                          ui::VKEY_PROCESSKEY,
                          key->flags());
    dispatch_details = DispatchKeyEventPostIME(&mock_key);
  } else {
    dispatch_details = DispatchKeyEventPostIME(key);
  }

  if (key->handled() || dispatch_details.dispatcher_destroyed)
    return dispatch_details;

  ui::TextInputClient* client = GetTextInputClient();
  if (client) {
    if (handled) {
      if (result_text_.length())
        client->InsertText(result_text_);
      if (composition_.text.length())
        client->SetCompositionText(composition_);
      else
        client->ClearCompositionText();
    } else if (key->type() == ui::ET_KEY_PRESSED) {
      base::char16 ch = key->GetCharacter();
      if (ch)
        client->InsertChar(*key);
    }
  }

  ClearComposition();

  return dispatch_details;
}
