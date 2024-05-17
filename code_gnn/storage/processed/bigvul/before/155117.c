bool OmniboxViewViews::HandleKeyEvent(views::Textfield* textfield,
                                      const ui::KeyEvent& event) {
  if (event.type() == ui::ET_KEY_RELEASED) {
    if (event.key_code() == ui::VKEY_CONTROL)
      model()->OnControlKeyChanged(false);

    return false;
  }

  if (event.IsUnicodeKeyCode())
    return false;

  const bool shift = event.IsShiftDown();
  const bool control = event.IsControlDown();
  const bool alt = event.IsAltDown() || event.IsAltGrDown();
  const bool command = event.IsCommandDown();
  switch (event.key_code()) {
    case ui::VKEY_RETURN:
      if (model()->popup_model()->selected_line_state() ==
          OmniboxPopupModel::BUTTON_FOCUSED) {
        if (!(OmniboxFieldTrial::IsTabSwitchLogicReversed() &&
              model()->popup_model()->SelectedLineHasTabMatch())) {
          popup_view_->OpenMatch(WindowOpenDisposition::SWITCH_TO_TAB,
                                 event.time_stamp());
        } else {
          popup_view_->OpenMatch(WindowOpenDisposition::CURRENT_TAB,
                                 event.time_stamp());
        }
      } else {
        if (alt || (shift && command)) {
          model()->AcceptInput(WindowOpenDisposition::NEW_FOREGROUND_TAB, false,
                               event.time_stamp());
        } else if (command) {
          model()->AcceptInput(WindowOpenDisposition::NEW_BACKGROUND_TAB, false,
                               event.time_stamp());
        } else if (shift) {
          model()->AcceptInput(WindowOpenDisposition::NEW_WINDOW, false,
                               event.time_stamp());
        } else {
          if (!(OmniboxFieldTrial::IsTabSwitchLogicReversed() &&
                model()->popup_model()->SelectedLineHasTabMatch())) {
            model()->AcceptInput(WindowOpenDisposition::CURRENT_TAB, false,
                                 event.time_stamp());
          } else {
            model()->AcceptInput(WindowOpenDisposition::SWITCH_TO_TAB, false,
                                 event.time_stamp());
          }
        }
      }
      return true;

    case ui::VKEY_ESCAPE:
      return model()->OnEscapeKeyPressed();

    case ui::VKEY_CONTROL:
      model()->OnControlKeyChanged(true);
      break;

    case ui::VKEY_DELETE:
      if (shift && model()->popup_model()->IsOpen()) {
        model()->popup_model()->TryDeletingLine(
            model()->popup_model()->selected_line());
      }
      break;

    case ui::VKEY_UP:
      if (shift)
        return false;

      if (IsTextEditCommandEnabled(ui::TextEditCommand::MOVE_UP)) {
        ExecuteTextEditCommand(ui::TextEditCommand::MOVE_UP);
        return true;
      }
      break;

    case ui::VKEY_DOWN:
      if (shift)
        return false;

      if (IsTextEditCommandEnabled(ui::TextEditCommand::MOVE_DOWN)) {
        ExecuteTextEditCommand(ui::TextEditCommand::MOVE_DOWN);
        return true;
      }
      break;

    case ui::VKEY_PRIOR:
      if (control || alt || shift || read_only())
        return false;
      model()->OnUpOrDownKeyPressed(-1 * model()->result().size());
      return true;

    case ui::VKEY_NEXT:
      if (control || alt || shift || read_only())
        return false;
      model()->OnUpOrDownKeyPressed(model()->result().size());
      return true;

    case ui::VKEY_RIGHT:
    case ui::VKEY_LEFT:
      if (control || alt || shift)
        return false;

      if (base::i18n::IsRTL() == (event.key_code() == ui::VKEY_LEFT)) {
        if (!DirectionAwareSelectionAtEnd())
          return false;

        if (OmniboxFieldTrial::IsExperimentalKeywordModeEnabled() &&
            model()->is_keyword_hint()) {
          OnBeforePossibleChange();
          model()->AcceptKeyword(OmniboxEventProto::SELECT_SUGGESTION);
          OnAfterPossibleChange(true);
          return true;
        } else if (MaybeFocusTabButton()) {
          return true;
        }
      } else if (MaybeUnfocusTabButton()) {
        return true;
      }
      break;

    case ui::VKEY_V:
      if (control && !alt &&
          IsTextEditCommandEnabled(ui::TextEditCommand::PASTE)) {
        ExecuteTextEditCommand(ui::TextEditCommand::PASTE);
        return true;
      }
      break;

    case ui::VKEY_INSERT:
      if (shift && !control &&
          IsTextEditCommandEnabled(ui::TextEditCommand::PASTE)) {
        ExecuteTextEditCommand(ui::TextEditCommand::PASTE);
        return true;
      }
      break;

    case ui::VKEY_BACK:
      if (model()->is_keyword_hint() || model()->keyword().empty() ||
          HasSelection() || GetCursorPosition() != 0)
        return false;
      model()->ClearKeyword();
      return true;

    case ui::VKEY_HOME:
      if (UnapplySteadyStateElisions(UnelisionGesture::HOME_KEY_PRESSED)) {
        if (shift) {
          size_t start, end;
          GetSelectionBounds(&start, &end);
          SelectRange(gfx::Range(start, 0));
        } else {
          SetCaretPos(0);
        }

        TextChanged();
        return true;
      }
      break;

    case ui::VKEY_SPACE:
      if (!(control || alt || shift)) {
        if (SelectionAtEnd() &&
            model()->popup_model()->selected_line_state() ==
                OmniboxPopupModel::BUTTON_FOCUSED) {
          popup_view_->OpenMatch(WindowOpenDisposition::SWITCH_TO_TAB,
                                 event.time_stamp());
          return true;
        }
      }
      break;

    default:
      break;
  }

  return HandleEarlyTabActions(event);
}
