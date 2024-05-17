void Textfield::ExecuteTextEditCommand(ui::TextEditCommand command) {
  DestroyTouchSelection();

  bool add_to_kill_buffer = false;

  switch (command) {
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_PARAGRAPH:
    case ui::TextEditCommand::DELETE_TO_END_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_END_OF_PARAGRAPH:
      add_to_kill_buffer = text_input_type_ != ui::TEXT_INPUT_TYPE_PASSWORD;
      FALLTHROUGH;
    case ui::TextEditCommand::DELETE_WORD_BACKWARD:
    case ui::TextEditCommand::DELETE_WORD_FORWARD:
      if (HasSelection())
        command = ui::TextEditCommand::DELETE_FORWARD;
      break;
    default:
      break;
  }

  if (!Textfield::IsTextEditCommandEnabled(command))
    return;

  bool text_changed = false;
  bool cursor_changed = false;
  bool rtl = GetTextDirection() == base::i18n::RIGHT_TO_LEFT;
  gfx::VisualCursorDirection begin = rtl ? gfx::CURSOR_RIGHT : gfx::CURSOR_LEFT;
  gfx::VisualCursorDirection end = rtl ? gfx::CURSOR_LEFT : gfx::CURSOR_RIGHT;
  gfx::SelectionModel selection_model = GetSelectionModel();

  OnBeforeUserAction();
  switch (command) {
    case ui::TextEditCommand::DELETE_BACKWARD:
      text_changed = cursor_changed = model_->Backspace(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::DELETE_FORWARD:
      text_changed = cursor_changed = model_->Delete(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_PARAGRAPH:
      model_->MoveCursor(gfx::LINE_BREAK, begin, gfx::SELECTION_RETAIN);
      text_changed = cursor_changed = model_->Backspace(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::DELETE_TO_END_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_END_OF_PARAGRAPH:
      model_->MoveCursor(gfx::LINE_BREAK, end, gfx::SELECTION_RETAIN);
      text_changed = cursor_changed = model_->Delete(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::DELETE_WORD_BACKWARD:
      model_->MoveCursor(gfx::WORD_BREAK, begin, gfx::SELECTION_RETAIN);
      text_changed = cursor_changed = model_->Backspace(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::DELETE_WORD_FORWARD:
      model_->MoveCursor(gfx::WORD_BREAK, end, gfx::SELECTION_RETAIN);
      text_changed = cursor_changed = model_->Delete(add_to_kill_buffer);
      break;
    case ui::TextEditCommand::MOVE_BACKWARD:
      model_->MoveCursor(gfx::CHARACTER_BREAK, begin, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_BACKWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::CHARACTER_BREAK, begin, gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_FORWARD:
      model_->MoveCursor(gfx::CHARACTER_BREAK, end, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_FORWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::CHARACTER_BREAK, end, gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_LEFT:
      model_->MoveCursor(gfx::CHARACTER_BREAK, gfx::CURSOR_LEFT,
                         gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_LEFT_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::CHARACTER_BREAK, gfx::CURSOR_LEFT,
                         gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_RIGHT:
      model_->MoveCursor(gfx::CHARACTER_BREAK, gfx::CURSOR_RIGHT,
                         gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_RIGHT_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::CHARACTER_BREAK, gfx::CURSOR_RIGHT,
                         gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_DOCUMENT:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_LINE:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_PARAGRAPH:
    case ui::TextEditCommand::MOVE_UP:
    case ui::TextEditCommand::MOVE_PAGE_UP:
      model_->MoveCursor(gfx::LINE_BREAK, begin, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::
        MOVE_TO_BEGINNING_OF_DOCUMENT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_LINE_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::
        MOVE_TO_BEGINNING_OF_PARAGRAPH_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, begin, kLineSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_PAGE_UP_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_UP_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, begin, gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_TO_END_OF_DOCUMENT:
    case ui::TextEditCommand::MOVE_TO_END_OF_LINE:
    case ui::TextEditCommand::MOVE_TO_END_OF_PARAGRAPH:
    case ui::TextEditCommand::MOVE_DOWN:
    case ui::TextEditCommand::MOVE_PAGE_DOWN:
      model_->MoveCursor(gfx::LINE_BREAK, end, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_TO_END_OF_DOCUMENT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_END_OF_LINE_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_END_OF_PARAGRAPH_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, end, kLineSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_PAGE_DOWN_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_DOWN_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, end, gfx::SELECTION_RETAIN);
      break;
    case ui::TextEditCommand::MOVE_PARAGRAPH_BACKWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, begin,
                         kMoveParagraphSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_PARAGRAPH_FORWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::LINE_BREAK, end, kMoveParagraphSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_WORD_BACKWARD:
      model_->MoveCursor(gfx::WORD_BREAK, begin, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_WORD_BACKWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::WORD_BREAK, begin, kWordSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_WORD_FORWARD:
      model_->MoveCursor(gfx::WORD_BREAK, end, gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_WORD_FORWARD_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::WORD_BREAK, end, kWordSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_WORD_LEFT:
      model_->MoveCursor(gfx::WORD_BREAK, gfx::CURSOR_LEFT,
                         gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_WORD_LEFT_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::WORD_BREAK, gfx::CURSOR_LEFT,
                         kWordSelectionBehavior);
      break;
    case ui::TextEditCommand::MOVE_WORD_RIGHT:
      model_->MoveCursor(gfx::WORD_BREAK, gfx::CURSOR_RIGHT,
                         gfx::SELECTION_NONE);
      break;
    case ui::TextEditCommand::MOVE_WORD_RIGHT_AND_MODIFY_SELECTION:
      model_->MoveCursor(gfx::WORD_BREAK, gfx::CURSOR_RIGHT,
                         kWordSelectionBehavior);
      break;
    case ui::TextEditCommand::UNDO:
      text_changed = cursor_changed = model_->Undo();
      break;
    case ui::TextEditCommand::REDO:
      text_changed = cursor_changed = model_->Redo();
      break;
    case ui::TextEditCommand::CUT:
      text_changed = cursor_changed = Cut();
      break;
    case ui::TextEditCommand::COPY:
      Copy();
      break;
    case ui::TextEditCommand::PASTE:
      text_changed = cursor_changed = Paste();
      break;
    case ui::TextEditCommand::SELECT_ALL:
      SelectAll(false);
      break;
    case ui::TextEditCommand::TRANSPOSE:
      text_changed = cursor_changed = model_->Transpose();
      break;
    case ui::TextEditCommand::YANK:
      text_changed = cursor_changed = model_->Yank();
      break;
    case ui::TextEditCommand::INSERT_TEXT:
    case ui::TextEditCommand::SET_MARK:
    case ui::TextEditCommand::UNSELECT:
    case ui::TextEditCommand::INVALID_COMMAND:
      NOTREACHED();
      break;
  }

  cursor_changed |= GetSelectionModel() != selection_model;
  if (cursor_changed && HasSelection())
    UpdateSelectionClipboard();
  UpdateAfterChange(text_changed, cursor_changed);
  OnAfterUserAction();
}
