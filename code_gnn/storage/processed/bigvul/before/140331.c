Range* Editor::findStringAndScrollToVisible(const String& target,
                                            Range* previousMatch,
                                            FindOptions options) {
  Range* nextMatch = findRangeOfString(
      target, EphemeralRangeInFlatTree(previousMatch), options);
  if (!nextMatch)
    return nullptr;

  Node* firstNode = nextMatch->firstNode();
  firstNode->layoutObject()->scrollRectToVisible(
      LayoutRect(nextMatch->boundingBox()),
      ScrollAlignment::alignCenterIfNeeded,
      ScrollAlignment::alignCenterIfNeeded, UserScroll);
  firstNode->document().setSequentialFocusNavigationStartingPoint(firstNode);

  return nextMatch;
}
