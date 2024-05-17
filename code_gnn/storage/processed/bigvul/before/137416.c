void RenderViewTest::SimulateUserTypingASCIICharacter(char ascii_character,
                                                      bool flush_message_loop) {
  int modifiers = blink::WebInputEvent::kNoModifiers;
  if (isupper(ascii_character) || ascii_character == '@' ||
      ascii_character == '_') {
    modifiers = blink::WebKeyboardEvent::kShiftKey;
  }

  blink::WebKeyboardEvent event(
      blink::WebKeyboardEvent::kRawKeyDown, modifiers,
      ui::EventTimeStampToSeconds(ui::EventTimeForNow()));
  event.text[0] = ascii_character;
  ASSERT_TRUE(GetWindowsKeyCode(ascii_character, &event.windows_key_code));
  SendWebKeyboardEvent(event);

  event.SetType(blink::WebKeyboardEvent::kChar);
  SendWebKeyboardEvent(event);

  event.SetType(blink::WebKeyboardEvent::kKeyUp);
  SendWebKeyboardEvent(event);

  if (flush_message_loop) {
    base::RunLoop().RunUntilIdle();
  }
}
