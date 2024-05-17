static Range* findRangeOfStringAlgorithm(
    Document& document,
    const String& target,
    const EphemeralRangeTemplate<Strategy>& referenceRange,
    FindOptions options) {
  if (target.isEmpty())
    return nullptr;

  EphemeralRangeTemplate<Strategy> documentRange =
      EphemeralRangeTemplate<Strategy>::rangeOfContents(document);
  EphemeralRangeTemplate<Strategy> searchRange(documentRange);

  bool forward = !(options & Backwards);
  bool startInReferenceRange = false;
  if (referenceRange.isNotNull()) {
    startInReferenceRange = options & StartInSelection;
    if (forward && startInReferenceRange)
      searchRange = EphemeralRangeTemplate<Strategy>(
          referenceRange.startPosition(), documentRange.endPosition());
    else if (forward)
      searchRange = EphemeralRangeTemplate<Strategy>(
          referenceRange.endPosition(), documentRange.endPosition());
    else if (startInReferenceRange)
      searchRange = EphemeralRangeTemplate<Strategy>(
          documentRange.startPosition(), referenceRange.endPosition());
    else
      searchRange = EphemeralRangeTemplate<Strategy>(
          documentRange.startPosition(), referenceRange.startPosition());
  }

  Range* resultRange = findStringBetweenPositions(target, searchRange, options);

  if (resultRange && startInReferenceRange &&
      normalizeRange(EphemeralRangeTemplate<Strategy>(resultRange)) ==
          referenceRange) {
    if (forward)
      searchRange = EphemeralRangeTemplate<Strategy>(
          fromPositionInDOMTree<Strategy>(resultRange->endPosition()),
          searchRange.endPosition());
    else
      searchRange = EphemeralRangeTemplate<Strategy>(
          searchRange.startPosition(),
          fromPositionInDOMTree<Strategy>(resultRange->startPosition()));
    resultRange = findStringBetweenPositions(target, searchRange, options);
  }

  if (!resultRange && options & WrapAround)
    return findStringBetweenPositions(target, documentRange, options);

  return resultRange;
}
