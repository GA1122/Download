void SimulateKeyPressImpl(WebContents* web_contents,
                          ui::DomKey key,
                          ui::DomCode code,
                          ui::KeyboardCode key_code,
                          bool control,
                          bool shift,
                          bool alt,
                          bool command,
                          bool send_char) {
  int modifiers =
      SimulateModifierKeysDown(web_contents, control, shift, alt, command);
  SimulateKeyEvent(web_contents, key, code, key_code, send_char, modifiers);
  modifiers = SimulateModifierKeysUp(web_contents, control, shift, alt, command,
                                     modifiers);
  ASSERT_EQ(modifiers, 0);
}
