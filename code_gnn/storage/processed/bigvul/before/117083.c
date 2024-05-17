bool SessionService::ReplacePendingCommand(SessionCommand* command) {
  if (command->id() != kCommandUpdateTabNavigation)
    return false;
  void* iterator = NULL;
  scoped_ptr<Pickle> command_pickle(command->PayloadAsPickle());
  SessionID::id_type command_tab_id;
  int command_nav_index;
  if (!command_pickle->ReadInt(&iterator, &command_tab_id) ||
      !command_pickle->ReadInt(&iterator, &command_nav_index)) {
    return false;
  }
  for (std::vector<SessionCommand*>::reverse_iterator i =
       pending_commands().rbegin(); i != pending_commands().rend(); ++i) {
    SessionCommand* existing_command = *i;
    if (existing_command->id() == kCommandUpdateTabNavigation) {
      SessionID::id_type existing_tab_id;
      int existing_nav_index;
      {
        scoped_ptr<Pickle> existing_pickle(existing_command->PayloadAsPickle());
        iterator = NULL;
        if (!existing_pickle->ReadInt(&iterator, &existing_tab_id) ||
            !existing_pickle->ReadInt(&iterator, &existing_nav_index)) {
          return false;
        }
      }
      if (existing_tab_id == command_tab_id &&
          existing_nav_index == command_nav_index) {
        delete existing_command;
        pending_commands().erase(i.base() - 1);
        pending_commands().push_back(command);
        return true;
      }
      return false;
    }
  }
  return false;
}
