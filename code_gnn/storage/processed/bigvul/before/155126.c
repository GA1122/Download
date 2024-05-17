bool OmniboxViewViews::IsTextEditCommandEnabled(
    ui::TextEditCommand command) const {
  switch (command) {
    case ui::TextEditCommand::MOVE_UP:
    case ui::TextEditCommand::MOVE_DOWN:
      return !read_only();
    case ui::TextEditCommand::PASTE:
      return !read_only() && !GetClipboardText().empty();
    default:
      return Textfield::IsTextEditCommandEnabled(command);
  }
}
