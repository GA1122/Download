void ImeObserver::OnSurroundingTextChanged(const std::string& component_id,
                                           const std::string& text,
                                           int cursor_pos,
                                           int anchor_pos,
                                           int offset_pos) {
  if (extension_id_.empty() ||
      !HasListener(input_ime::OnSurroundingTextChanged::kEventName))
    return;

  input_ime::OnSurroundingTextChanged::SurroundingInfo info;
  info.text = text;
  info.focus = cursor_pos;
  info.anchor = anchor_pos;
  info.offset = offset_pos;
  std::unique_ptr<base::ListValue> args(
      input_ime::OnSurroundingTextChanged::Create(component_id, info));

  DispatchEventToExtension(
    extensions::events::INPUT_IME_ON_SURROUNDING_TEXT_CHANGED,
    input_ime::OnSurroundingTextChanged::kEventName, std::move(args));
}
