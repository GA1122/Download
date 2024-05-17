void AppendCharEvent(const WebInputEvent& event,
                     std::vector<PP_InputEvent>* pp_events) {
  const WebKeyboardEvent& key_event =
      reinterpret_cast<const WebKeyboardEvent&>(event);

  size_t utf16_char_count = 0;
  while (utf16_char_count < WebKeyboardEvent::textLengthCap &&
         key_event.text[utf16_char_count])
    utf16_char_count++;

  base::i18n::UTF16CharIterator iter(key_event.text, utf16_char_count);
  while (!iter.end()) {
    PP_InputEvent result = GetPPEventWithCommonFieldsAndType(event);
    result.u.character.modifier = key_event.modifiers;

    std::string utf8_char;
    base::WriteUnicodeCharacter(iter.get(), &utf8_char);
    base::strlcpy(result.u.character.text, utf8_char.c_str(),
                  sizeof(result.u.character.text));

    pp_events->push_back(result);
    iter.Advance();
  }
}
