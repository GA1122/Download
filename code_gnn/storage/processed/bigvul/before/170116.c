void InputConnectionImpl::SendControlKeyEvent(const base::string16& text) {
  DCHECK(IsControlChar(text));

  const std::string str = base::UTF16ToUTF8(text);
  DCHECK_EQ(1u, str.length());

  for (const auto& t : kControlCharToKeyEvent) {
    if (std::get<0>(t) == str[0]) {
      chromeos::InputMethodEngine::KeyboardEvent press;
      press.type = "keydown";
      press.key_code = std::get<1>(t);
      press.key = press.code = std::get<2>(t);
      chromeos::InputMethodEngine::KeyboardEvent release(press);
      release.type = "keyup";
      ime_engine_->SendKeyEvents(input_context_id_, {press, release});
      break;
    }
  }
  return;
}
