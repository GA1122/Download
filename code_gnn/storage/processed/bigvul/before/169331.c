void SimulateKeyPress(WebContents* web_contents,
                      ui::DomKey key,
                      ui::DomCode code,
                      ui::KeyboardCode key_code,
                      bool control,
                      bool shift,
                      bool alt,
                      bool command) {
  SimulateKeyPressImpl(web_contents, key, code, key_code, control, shift, alt,
                       command,  true);
}
