bool InputMethodIBus::ExecuteCharacterComposer(uint32 ibus_keyval,
                                               uint32 ibus_keycode,
                                               uint32 ibus_state) {
  bool consumed = character_composer_.FilterKeyPress(
      ibus_keyval,
      ibus_keycode,
      EventFlagsFromXState(ibus_state));

  suppress_next_result_ = false;
  chromeos::IBusText preedit;
  preedit.set_text(
      UTF16ToUTF8(character_composer_.preedit_string()));
  UpdatePreeditText(preedit, preedit.text().size(),
                    !preedit.text().empty());
   std::string commit_text =
      UTF16ToUTF8(character_composer_.composed_character());
  if (!commit_text.empty()) {
    CommitText(commit_text);
  }
  return consumed;
}
