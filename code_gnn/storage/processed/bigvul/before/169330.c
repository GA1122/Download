void SimulateKeyEvent(WebContents* web_contents,
                      ui::DomKey key,
                      ui::DomCode code,
                      ui::KeyboardCode key_code,
                      bool send_char,
                      int modifiers) {
  InjectRawKeyEvent(web_contents, blink::WebInputEvent::kRawKeyDown, key, code,
                    key_code, modifiers);
  if (send_char) {
    InjectRawKeyEvent(web_contents, blink::WebInputEvent::kChar, key, code,
                      key_code, modifiers);
  }
  InjectRawKeyEvent(web_contents, blink::WebInputEvent::kKeyUp, key, code,
                    key_code, modifiers);
}
