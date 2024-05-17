bool Editor::findString(const String& target, FindOptions options) {
  VisibleSelection selection =
      frame().selection().computeVisibleSelectionInDOMTreeDeprecated();

  Range* resultRange = findRangeOfString(
      target, EphemeralRange(selection.start(), selection.end()),
      static_cast<FindOptions>(options | FindAPICall));

  if (!resultRange)
    return false;

  frame().selection().setSelection(
      SelectionInDOMTree::Builder()
          .setBaseAndExtent(EphemeralRange(resultRange))
          .build());
  frame().selection().revealSelection();
  return true;
}
