const StaticRangeVector* Editor::Command::GetTargetRanges() const {
  const Node* target = EventTargetNodeForDocument(frame_->GetDocument());
  if (!IsSupported() || !frame_ || !target || !HasRichlyEditableStyle(*target))
    return nullptr;

  switch (command_->command_type) {
    case WebEditingCommandType::kDelete:
    case WebEditingCommandType::kDeleteBackward:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kBackward,
          TextGranularity::kCharacter);
    case WebEditingCommandType::kDeleteForward:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kForward,
          TextGranularity::kCharacter);
    case WebEditingCommandType::kDeleteToBeginningOfLine:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kBackward, TextGranularity::kLine);
    case WebEditingCommandType::kDeleteToBeginningOfParagraph:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kBackward,
          TextGranularity::kParagraph);
    case WebEditingCommandType::kDeleteToEndOfLine:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kForward, TextGranularity::kLine);
    case WebEditingCommandType::kDeleteToEndOfParagraph:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kForward,
          TextGranularity::kParagraph);
    case WebEditingCommandType::kDeleteWordBackward:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kBackward, TextGranularity::kWord);
    case WebEditingCommandType::kDeleteWordForward:
      return RangesFromCurrentSelectionOrExtendCaret(
          *frame_, SelectionModifyDirection::kForward, TextGranularity::kWord);
    default:
      return TargetRangesForInputEvent(*target);
  }
}
