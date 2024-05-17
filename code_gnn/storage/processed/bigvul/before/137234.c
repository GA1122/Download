bool Textfield::IsCommandIdChecked(int command_id) const {
  if (text_services_context_menu_ &&
      text_services_context_menu_->SupportsCommand(command_id)) {
    return text_services_context_menu_->IsCommandIdChecked(command_id);
  }

  return true;
}
