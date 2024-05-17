bool Textfield::OnKeyPressed(const ui::KeyEvent& event) {
  ui::TextEditCommand edit_command = scheduled_text_edit_command_;
  scheduled_text_edit_command_ = ui::TextEditCommand::INVALID_COMMAND;

  base::WeakPtr<Textfield> textfield(weak_ptr_factory_.GetWeakPtr());

  bool handled = controller_ && controller_->HandleKeyEvent(this, event);

  if (!textfield)
    return handled;

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  ui::TextEditKeyBindingsDelegateAuraLinux* delegate =
      ui::GetTextEditKeyBindingsDelegate();
  std::vector<ui::TextEditCommandAuraLinux> commands;
  if (!handled && delegate && delegate->MatchEvent(event, &commands)) {
    for (size_t i = 0; i < commands.size(); ++i) {
      if (IsTextEditCommandEnabled(commands[i].command())) {
        ExecuteTextEditCommand(commands[i].command());
        handled = true;
      }
    }
    return handled;
  }
#endif

  if (edit_command == ui::TextEditCommand::INVALID_COMMAND)
    edit_command = GetCommandForKeyEvent(event);

  if (!handled && IsTextEditCommandEnabled(edit_command)) {
    ExecuteTextEditCommand(edit_command);
    handled = true;
  }
  return handled;
}
