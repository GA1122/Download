void Editor::ReplaceSelectionWithText(const String& text,
                                      bool select_replacement,
                                      bool smart_replace,
                                      InputEvent::InputType input_type) {
  ReplaceSelectionWithFragment(CreateFragmentFromText(SelectedRange(), text),
                               select_replacement, smart_replace, true,
                               input_type);
}
