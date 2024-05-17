void Editor::transpose() {
  if (!canEdit())
    return;

  VisibleSelection selection =
      frame().selection().computeVisibleSelectionInDOMTreeDeprecated();
  if (!selection.isCaret())
    return;

  VisiblePosition caret = selection.visibleStart();
  VisiblePosition next =
      isEndOfParagraph(caret) ? caret : nextPositionOf(caret);
  VisiblePosition previous = previousPositionOf(next);
  if (next.deepEquivalent() == previous.deepEquivalent())
    return;
  previous = previousPositionOf(previous);
  if (!inSameParagraph(next, previous))
    return;
  const EphemeralRange range = makeRange(previous, next);
  if (range.isNull())
    return;
  const SelectionInDOMTree newSelection =
      SelectionInDOMTree::Builder().setBaseAndExtent(range).build();

  String text = plainText(range);
  if (text.length() != 2)
    return;
  String transposed = text.right(1) + text.left(1);

  if (createVisibleSelection(newSelection) !=
      frame().selection().computeVisibleSelectionInDOMTreeDeprecated())
    frame().selection().setSelection(newSelection);

  replaceSelectionWithText(transposed, false, false,
                           InputEvent::InputType::InsertFromPaste);
}
