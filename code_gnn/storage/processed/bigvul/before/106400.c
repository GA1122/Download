WebKeyboardEvent* BuildCharEvent(const PP_InputEvent& event) {
  WebKeyboardEvent* key_event = new WebKeyboardEvent();
  key_event->type = WebInputEvent::Char;
  key_event->timeStampSeconds = event.time_stamp;
  key_event->modifiers = event.u.character.modifier;

  size_t text_length_cap = WebKeyboardEvent::textLengthCap;
  size_t text_len = 0;
  while (text_len < text_length_cap && event.u.character.text[text_len])
      text_len++;
  string16 text16 = UTF8ToUTF16(std::string(event.u.character.text, text_len));

  memset(key_event->text, 0, text_length_cap);
  memset(key_event->unmodifiedText, 0, text_length_cap);
  for (size_t i = 0;
       i < std::min(text_length_cap, text16.size());
       ++i)
    key_event->text[i] = text16[i];
  return key_event;
}
