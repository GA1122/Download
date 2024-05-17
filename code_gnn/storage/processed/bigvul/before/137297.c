void Textfield::SetTextEditCommandForNextKeyEvent(ui::TextEditCommand command) {
  DCHECK_EQ(ui::TextEditCommand::INVALID_COMMAND, scheduled_text_edit_command_);
  scheduled_text_edit_command_ = command;
}
