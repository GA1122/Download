bool Editor::Command::IsTextInsertion() const {
  return command_ && command_->is_text_insertion;
}
