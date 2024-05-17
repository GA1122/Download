InputEvent::InputType InputTypeFromCommandType(
    WebEditingCommandType command_type,
    LocalFrame& frame) {
  using CommandType = WebEditingCommandType;
  using InputType = InputEvent::InputType;

  if (command_type == CommandType::kInsertNewline)
    return frame.GetEditor().CanEditRichly() ? InputType::kInsertParagraph
                                             : InputType::kInsertLineBreak;

  switch (command_type) {
    case CommandType::kInsertBacktab:
    case CommandType::kInsertText:
      return InputType::kInsertText;
    case CommandType::kInsertLineBreak:
      return InputType::kInsertLineBreak;
    case CommandType::kInsertParagraph:
    case CommandType::kInsertNewlineInQuotedContent:
      return InputType::kInsertParagraph;
    case CommandType::kInsertHorizontalRule:
      return InputType::kInsertHorizontalRule;
    case CommandType::kInsertOrderedList:
      return InputType::kInsertOrderedList;
    case CommandType::kInsertUnorderedList:
      return InputType::kInsertUnorderedList;

    case CommandType::kDelete:
    case CommandType::kDeleteBackward:
    case CommandType::kDeleteBackwardByDecomposingPreviousCharacter:
      return InputType::kDeleteContentBackward;
    case CommandType::kDeleteForward:
      return InputType::kDeleteContentForward;
    case CommandType::kDeleteToBeginningOfLine:
      return InputType::kDeleteSoftLineBackward;
    case CommandType::kDeleteToEndOfLine:
      return InputType::kDeleteSoftLineForward;
    case CommandType::kDeleteWordBackward:
      return InputType::kDeleteWordBackward;
    case CommandType::kDeleteWordForward:
      return InputType::kDeleteWordForward;
    case CommandType::kDeleteToBeginningOfParagraph:
      return InputType::kDeleteHardLineBackward;
    case CommandType::kDeleteToEndOfParagraph:
      return InputType::kDeleteHardLineForward;
    case CommandType::kDeleteToMark:
      return InputType::kNone;

    case CommandType::kUndo:
      return InputType::kHistoryUndo;
    case CommandType::kRedo:
      return InputType::kHistoryRedo;

    case CommandType::kBold:
    case CommandType::kToggleBold:
      return InputType::kFormatBold;
    case CommandType::kItalic:
    case CommandType::kToggleItalic:
      return InputType::kFormatItalic;
    case CommandType::kUnderline:
    case CommandType::kToggleUnderline:
      return InputType::kFormatUnderline;
    case CommandType::kStrikethrough:
      return InputType::kFormatStrikeThrough;
    case CommandType::kSuperscript:
      return InputType::kFormatSuperscript;
    case CommandType::kSubscript:
      return InputType::kFormatSubscript;
    default:
      return InputType::kNone;
  }
}
