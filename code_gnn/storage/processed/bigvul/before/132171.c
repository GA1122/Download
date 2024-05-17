void RenderViewTest::SimulateUserTypingASCIICharacter(char ascii_character,
                                                      bool flush_message_loop) {
  blink::WebKeyboardEvent event;
  event.text[0] = ascii_character;
  ASSERT_TRUE(GetWindowsKeyCode(ascii_character, &event.windowsKeyCode));
  if (isupper(ascii_character) || ascii_character == '@' ||
      ascii_character == '_') {
    event.modifiers = blink::WebKeyboardEvent::ShiftKey;
  }

  event.type = blink::WebKeyboardEvent::RawKeyDown;
  SendWebKeyboardEvent(event);

  event.type = blink::WebKeyboardEvent::Char;
  SendWebKeyboardEvent(event);

  event.type = blink::WebKeyboardEvent::KeyUp;
  SendWebKeyboardEvent(event);

  if (flush_message_loop) {
    base::MessageLoop::current()->RunUntilIdle();
  }
}
