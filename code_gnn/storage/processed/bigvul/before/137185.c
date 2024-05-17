ui::TextEditCommand GetCommandForKeyEvent(const ui::KeyEvent& event) {
  if (event.type() != ui::ET_KEY_PRESSED || event.IsUnicodeKeyCode())
    return ui::TextEditCommand::INVALID_COMMAND;

  const bool shift = event.IsShiftDown();
  const bool control = event.IsControlDown() || event.IsCommandDown();
  const bool alt = event.IsAltDown() || event.IsAltGrDown();
  switch (event.key_code()) {
    case ui::VKEY_Z:
      if (control && !shift && !alt)
        return ui::TextEditCommand::UNDO;
      return (control && shift && !alt) ? ui::TextEditCommand::REDO
                                        : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_Y:
      return (control && !alt) ? ui::TextEditCommand::REDO
                               : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_A:
      return (control && !alt) ? ui::TextEditCommand::SELECT_ALL
                               : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_X:
      return (control && !alt) ? ui::TextEditCommand::CUT
                               : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_C:
      return (control && !alt) ? ui::TextEditCommand::COPY
                               : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_V:
      return (control && !alt) ? ui::TextEditCommand::PASTE
                               : ui::TextEditCommand::INVALID_COMMAND;
    case ui::VKEY_RIGHT:
      if (alt)
        return ui::TextEditCommand::INVALID_COMMAND;
      if (!shift) {
        return control ? ui::TextEditCommand::MOVE_WORD_RIGHT
                       : ui::TextEditCommand::MOVE_RIGHT;
      }
      return control ? ui::TextEditCommand::MOVE_WORD_RIGHT_AND_MODIFY_SELECTION
                     : ui::TextEditCommand::MOVE_RIGHT_AND_MODIFY_SELECTION;
    case ui::VKEY_LEFT:
      if (alt)
        return ui::TextEditCommand::INVALID_COMMAND;
      if (!shift) {
        return control ? ui::TextEditCommand::MOVE_WORD_LEFT
                       : ui::TextEditCommand::MOVE_LEFT;
      }
      return control ? ui::TextEditCommand::MOVE_WORD_LEFT_AND_MODIFY_SELECTION
                     : ui::TextEditCommand::MOVE_LEFT_AND_MODIFY_SELECTION;
    case ui::VKEY_HOME:
      return shift ? ui::TextEditCommand::
                         MOVE_TO_BEGINNING_OF_LINE_AND_MODIFY_SELECTION
                   : ui::TextEditCommand::MOVE_TO_BEGINNING_OF_LINE;
    case ui::VKEY_END:
      return shift
                 ? ui::TextEditCommand::MOVE_TO_END_OF_LINE_AND_MODIFY_SELECTION
                 : ui::TextEditCommand::MOVE_TO_END_OF_LINE;
    case ui::VKEY_BACK:
      if (!control)
        return ui::TextEditCommand::DELETE_BACKWARD;
#if defined(OS_LINUX)
      if (shift)
        return ui::TextEditCommand::DELETE_TO_BEGINNING_OF_LINE;
#endif
      return ui::TextEditCommand::DELETE_WORD_BACKWARD;
    case ui::VKEY_DELETE:
#if defined(OS_LINUX)
      if (shift && control)
        return ui::TextEditCommand::DELETE_TO_END_OF_LINE;
#endif
      if (control)
        return ui::TextEditCommand::DELETE_WORD_FORWARD;
      return shift ? ui::TextEditCommand::CUT
                   : ui::TextEditCommand::DELETE_FORWARD;
    case ui::VKEY_INSERT:
      if (control && !shift)
        return ui::TextEditCommand::COPY;
      return (shift && !control) ? ui::TextEditCommand::PASTE
                                 : ui::TextEditCommand::INVALID_COMMAND;
    default:
      return ui::TextEditCommand::INVALID_COMMAND;
  }
}
