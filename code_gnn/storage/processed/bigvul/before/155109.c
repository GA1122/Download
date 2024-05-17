void OmniboxViewViews::ExecuteTextEditCommand(ui::TextEditCommand command) {
  DestroyTouchSelection();

  if (!IsTextEditCommandEnabled(command))
    return;

  switch (command) {
    case ui::TextEditCommand::MOVE_UP:
      model()->OnUpOrDownKeyPressed(-1);
      break;
    case ui::TextEditCommand::MOVE_DOWN:
      model()->OnUpOrDownKeyPressed(1);
      break;
    case ui::TextEditCommand::PASTE:
      OnPaste();
      break;
    default:
      Textfield::ExecuteTextEditCommand(command);
      break;
  }
 }
