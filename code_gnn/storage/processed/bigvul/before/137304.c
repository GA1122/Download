bool Textfield::SkipDefaultKeyEventProcessing(const ui::KeyEvent& event) {
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  ui::TextEditKeyBindingsDelegateAuraLinux* delegate =
      ui::GetTextEditKeyBindingsDelegate();
  std::vector<ui::TextEditCommandAuraLinux> commands;
  if (delegate && delegate->MatchEvent(event, &commands)) {
    for (size_t i = 0; i < commands.size(); ++i)
      if (IsTextEditCommandEnabled(commands[i].command()))
        return true;
  }
#endif

  const bool is_backspace = event.key_code() == ui::VKEY_BACK;
  return (is_backspace && !read_only()) || event.IsUnicodeKeyCode();
}
