bool OmniboxViewViews::OnKeyPressed(const ui::KeyEvent& event) {
  if (event.IsUnicodeKeyCode())
    return views::Textfield::OnKeyPressed(event);

  const bool shift = event.IsShiftDown();
  const bool control = event.IsControlDown();
  const bool alt = event.IsAltDown() || event.IsAltGrDown();
  switch (event.key_code()) {
    case ui::VKEY_RETURN:
      model()->AcceptInput(alt ? NEW_FOREGROUND_TAB : CURRENT_TAB, false);
      return true;
    case ui::VKEY_ESCAPE:
      return model()->OnEscapeKeyPressed();
    case ui::VKEY_CONTROL:
      model()->OnControlKeyChanged(true);
      break;
    case ui::VKEY_DELETE:
      if (shift && model()->popup_model()->IsOpen())
        model()->popup_model()->TryDeletingCurrentItem();
      break;
    case ui::VKEY_UP:
      if (!read_only()) {
        model()->OnUpOrDownKeyPressed(-1);
        return true;
      }
      break;
    case ui::VKEY_DOWN:
      if (!read_only()) {
        model()->OnUpOrDownKeyPressed(1);
        return true;
      }
      break;
    case ui::VKEY_PRIOR:
      if (control || alt || shift)
        return false;
      model()->OnUpOrDownKeyPressed(-1 * model()->result().size());
      return true;
    case ui::VKEY_NEXT:
      if (control || alt || shift)
        return false;
      model()->OnUpOrDownKeyPressed(model()->result().size());
      return true;
    case ui::VKEY_V:
      if (control && !alt && !read_only()) {
        ExecuteCommand(IDS_APP_PASTE, 0);
        return true;
      }
      break;
    case ui::VKEY_INSERT:
      if (shift && !control && !read_only()) {
        ExecuteCommand(IDS_APP_PASTE, 0);
        return true;
      }
      break;
    default:
      break;
  }

  return views::Textfield::OnKeyPressed(event) || HandleEarlyTabActions(event);
}
