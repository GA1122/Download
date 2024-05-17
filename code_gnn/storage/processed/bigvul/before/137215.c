ui::TextEditCommand GetTextEditCommandFromMenuCommand(int command_id,
                                                      bool has_selection) {
  switch (command_id) {
    case IDS_APP_UNDO:
      return ui::TextEditCommand::UNDO;
    case IDS_APP_CUT:
      return ui::TextEditCommand::CUT;
    case IDS_APP_COPY:
      return ui::TextEditCommand::COPY;
    case IDS_APP_PASTE:
      return ui::TextEditCommand::PASTE;
    case IDS_APP_DELETE:
      if (has_selection)
        return ui::TextEditCommand::DELETE_FORWARD;
      break;
    case IDS_APP_SELECT_ALL:
      return ui::TextEditCommand::SELECT_ALL;
  }
  return ui::TextEditCommand::INVALID_COMMAND;
}
