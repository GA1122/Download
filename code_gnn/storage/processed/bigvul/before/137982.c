AXObject::AXRange AXLayoutObject::selectionUnderObject() const {
  AXRange textSelection = textControlSelection();
  if (textSelection.isValid())
    return textSelection;

  if (!getNode() || !getLayoutObject()->frame())
    return AXRange();

  VisibleSelection selection =
      getLayoutObject()
          ->frame()
          ->selection()
          .computeVisibleSelectionInDOMTreeDeprecated();
  Range* selectionRange = createRange(firstEphemeralRangeOf(selection));
  ContainerNode* parentNode = getNode()->parentNode();
  int nodeIndex = getNode()->nodeIndex();
  if (!selectionRange
      ||
      !(parentNode &&
        selectionRange->comparePoint(parentNode, nodeIndex,
                                     IGNORE_EXCEPTION_FOR_TESTING) < 0 &&
        selectionRange->comparePoint(parentNode, nodeIndex + 1,
                                     IGNORE_EXCEPTION_FOR_TESTING) > 0)) {
    return AXRange();
  }

  int start = indexForVisiblePosition(selection.visibleStart());
  ASSERT(start >= 0);
  int end = indexForVisiblePosition(selection.visibleEnd());
  ASSERT(end >= 0);

  return AXRange(start, end);
}
