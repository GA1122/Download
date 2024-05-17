bool Textfield::IsCommandIdEnabled(int command_id) const {
  if (text_services_context_menu_ &&
      text_services_context_menu_->SupportsCommand(command_id)) {
    return text_services_context_menu_->IsCommandIdEnabled(command_id);
  }

  return Textfield::IsTextEditCommandEnabled(
      GetTextEditCommandFromMenuCommand(command_id, HasSelection()));
}
