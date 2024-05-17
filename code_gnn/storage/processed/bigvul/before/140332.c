static Range* findStringBetweenPositions(
    const String& target,
    const EphemeralRangeTemplate<Strategy>& referenceRange,
    FindOptions options) {
  EphemeralRangeTemplate<Strategy> searchRange(referenceRange);

  bool forward = !(options & Backwards);

  while (true) {
    EphemeralRangeTemplate<Strategy> resultRange =
        findPlainText(searchRange, target, options);
    if (resultRange.isCollapsed())
      return nullptr;

    Range* rangeObject =
        Range::create(resultRange.document(),
                      toPositionInDOMTree(resultRange.startPosition()),
                      toPositionInDOMTree(resultRange.endPosition()));
    if (!rangeObject->collapsed())
      return rangeObject;

    if (forward) {
      searchRange = EphemeralRangeTemplate<Strategy>(
          nextPositionOf(resultRange.startPosition(),
                         PositionMoveType::GraphemeCluster),
          searchRange.endPosition());
    } else {
      searchRange = EphemeralRangeTemplate<Strategy>(
          searchRange.startPosition(),
          previousPositionOf(resultRange.endPosition(),
                             PositionMoveType::GraphemeCluster));
    }
  }

  NOTREACHED();
  return nullptr;
}
