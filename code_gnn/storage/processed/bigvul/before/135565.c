bool Editor::InsertParagraphSeparator() {
  if (!CanEdit())
    return false;

  if (!CanEditRichly())
    return InsertLineBreak();

  VisiblePosition caret =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree().VisibleStart();
  bool align_to_edge = IsEndOfEditableOrNonEditableContent(caret);
  DCHECK(GetFrame().GetDocument());
  EditingState editing_state;
  if (!TypingCommand::InsertParagraphSeparator(*GetFrame().GetDocument()))
    return false;
  RevealSelectionAfterEditingOperation(
      align_to_edge ? ScrollAlignment::kAlignToEdgeIfNeeded
                    : ScrollAlignment::kAlignCenterIfNeeded);

  return true;
}
