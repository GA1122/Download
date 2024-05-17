InputEvent::InputType TypingCommand::inputType() const {
  using InputType = InputEvent::InputType;

  if (m_compositionType != TextCompositionNone)
    return InputType::InsertCompositionText;

  switch (m_commandType) {
    case DeleteSelection:
      return InputType::DeleteContentBackward;
    case DeleteKey:
      return deletionInputTypeFromTextGranularity(DeleteDirection::Backward,
                                                  m_granularity);
    case ForwardDeleteKey:
      return deletionInputTypeFromTextGranularity(DeleteDirection::Forward,
                                                  m_granularity);
    case InsertText:
      return InputType::InsertText;
    case InsertLineBreak:
      return InputType::InsertLineBreak;
    case InsertParagraphSeparator:
    case InsertParagraphSeparatorInQuotedContent:
      return InputType::InsertParagraph;
    default:
      return InputType::None;
  }
}
