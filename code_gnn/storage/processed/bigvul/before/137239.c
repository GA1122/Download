bool Textfield::IsTextEditCommandEnabled(ui::TextEditCommand command) const {
  base::string16 result;
  bool editable = !read_only();
  bool readable = text_input_type_ != ui::TEXT_INPUT_TYPE_PASSWORD;
  switch (command) {
    case ui::TextEditCommand::DELETE_BACKWARD:
    case ui::TextEditCommand::DELETE_FORWARD:
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_BEGINNING_OF_PARAGRAPH:
    case ui::TextEditCommand::DELETE_TO_END_OF_LINE:
    case ui::TextEditCommand::DELETE_TO_END_OF_PARAGRAPH:
    case ui::TextEditCommand::DELETE_WORD_BACKWARD:
    case ui::TextEditCommand::DELETE_WORD_FORWARD:
      return editable;
    case ui::TextEditCommand::MOVE_BACKWARD:
    case ui::TextEditCommand::MOVE_BACKWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_FORWARD:
    case ui::TextEditCommand::MOVE_FORWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_LEFT:
    case ui::TextEditCommand::MOVE_LEFT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_RIGHT:
    case ui::TextEditCommand::MOVE_RIGHT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_DOCUMENT:
    case ui::TextEditCommand::
        MOVE_TO_BEGINNING_OF_DOCUMENT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_LINE:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_LINE_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_BEGINNING_OF_PARAGRAPH:
    case ui::TextEditCommand::
        MOVE_TO_BEGINNING_OF_PARAGRAPH_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_END_OF_DOCUMENT:
    case ui::TextEditCommand::MOVE_TO_END_OF_DOCUMENT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_END_OF_LINE:
    case ui::TextEditCommand::MOVE_TO_END_OF_LINE_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_TO_END_OF_PARAGRAPH:
    case ui::TextEditCommand::MOVE_TO_END_OF_PARAGRAPH_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_PARAGRAPH_FORWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_PARAGRAPH_BACKWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_WORD_BACKWARD:
    case ui::TextEditCommand::MOVE_WORD_BACKWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_WORD_FORWARD:
    case ui::TextEditCommand::MOVE_WORD_FORWARD_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_WORD_LEFT:
    case ui::TextEditCommand::MOVE_WORD_LEFT_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_WORD_RIGHT:
    case ui::TextEditCommand::MOVE_WORD_RIGHT_AND_MODIFY_SELECTION:
      return true;
    case ui::TextEditCommand::UNDO:
      return editable && model_->CanUndo();
    case ui::TextEditCommand::REDO:
      return editable && model_->CanRedo();
    case ui::TextEditCommand::CUT:
      return editable && readable && model_->HasSelection();
    case ui::TextEditCommand::COPY:
      return readable && model_->HasSelection();
    case ui::TextEditCommand::PASTE:
      ui::Clipboard::GetForCurrentThread()->ReadText(
          ui::CLIPBOARD_TYPE_COPY_PASTE, &result);
      return editable && !result.empty();
    case ui::TextEditCommand::SELECT_ALL:
      return !text().empty() && GetSelectedRange().length() != text().length();
    case ui::TextEditCommand::TRANSPOSE:
      return editable && !model_->HasSelection() &&
             !model_->HasCompositionText();
    case ui::TextEditCommand::YANK:
      return editable;
    case ui::TextEditCommand::MOVE_DOWN:
    case ui::TextEditCommand::MOVE_DOWN_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_PAGE_DOWN:
    case ui::TextEditCommand::MOVE_PAGE_DOWN_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_PAGE_UP:
    case ui::TextEditCommand::MOVE_PAGE_UP_AND_MODIFY_SELECTION:
    case ui::TextEditCommand::MOVE_UP:
    case ui::TextEditCommand::MOVE_UP_AND_MODIFY_SELECTION:
#if defined(OS_MACOSX)
      return true;
#else
      return false;
#endif
    case ui::TextEditCommand::INSERT_TEXT:
    case ui::TextEditCommand::SET_MARK:
    case ui::TextEditCommand::UNSELECT:
    case ui::TextEditCommand::INVALID_COMMAND:
      return false;
  }
  NOTREACHED();
  return false;
}
