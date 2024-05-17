static Range* FindRangeOfStringAlgorithm(
    Document& document,
    const String& target,
    const EphemeralRangeTemplate<Strategy>& reference_range,
    FindOptions options) {
  if (target.IsEmpty())
    return nullptr;

  EphemeralRangeTemplate<Strategy> document_range =
      EphemeralRangeTemplate<Strategy>::RangeOfContents(document);
  EphemeralRangeTemplate<Strategy> search_range(document_range);

  bool forward = !(options & kBackwards);
  bool start_in_reference_range = false;
  if (reference_range.IsNotNull()) {
    start_in_reference_range = options & kStartInSelection;
    if (forward && start_in_reference_range)
      search_range = EphemeralRangeTemplate<Strategy>(
          reference_range.StartPosition(), document_range.EndPosition());
    else if (forward)
      search_range = EphemeralRangeTemplate<Strategy>(
          reference_range.EndPosition(), document_range.EndPosition());
    else if (start_in_reference_range)
      search_range = EphemeralRangeTemplate<Strategy>(
          document_range.StartPosition(), reference_range.EndPosition());
    else
      search_range = EphemeralRangeTemplate<Strategy>(
          document_range.StartPosition(), reference_range.StartPosition());
  }

  Range* result_range =
      FindStringBetweenPositions(target, search_range, options);

  if (result_range && start_in_reference_range &&
      NormalizeRange(EphemeralRangeTemplate<Strategy>(result_range)) ==
          reference_range) {
    if (forward)
      search_range = EphemeralRangeTemplate<Strategy>(
          FromPositionInDOMTree<Strategy>(result_range->EndPosition()),
          search_range.EndPosition());
    else
      search_range = EphemeralRangeTemplate<Strategy>(
          search_range.StartPosition(),
          FromPositionInDOMTree<Strategy>(result_range->StartPosition()));
    result_range = FindStringBetweenPositions(target, search_range, options);
  }

  if (!result_range && options & kWrapAround)
    return FindStringBetweenPositions(target, document_range, options);

  return result_range;
}
