SelectionInDOMTree createSelection(const size_t start,
                                   const size_t end,
                                   const bool isDirectional,
                                   Element* element) {
  const EphemeralRange& startRange =
      PlainTextRange(0, static_cast<int>(start)).createRange(*element);
  DCHECK(startRange.isNotNull());
  const Position& startPosition = startRange.endPosition();

  const EphemeralRange& endRange =
      PlainTextRange(0, static_cast<int>(end)).createRange(*element);
  DCHECK(endRange.isNotNull());
  const Position& endPosition = endRange.endPosition();

  const SelectionInDOMTree& selection =
      SelectionInDOMTree::Builder()
          .setBaseAndExtent(startPosition, endPosition)
          .setIsDirectional(isDirectional)
          .build();
  return selection;
}
