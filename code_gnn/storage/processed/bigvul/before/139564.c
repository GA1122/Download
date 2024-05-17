static EphemeralRange UnionEphemeralRanges(const EphemeralRange& range1,
                                           const EphemeralRange& range2) {
  const Position start_position =
      range1.StartPosition().CompareTo(range2.StartPosition()) <= 0
          ? range1.StartPosition()
          : range2.StartPosition();
  const Position end_position =
      range1.EndPosition().CompareTo(range2.EndPosition()) <= 0
          ? range1.EndPosition()
          : range2.EndPosition();
  return EphemeralRange(start_position, end_position);
}
