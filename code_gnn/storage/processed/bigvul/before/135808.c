static int TextDistance(const PositionInFlatTree& start,
                        const PositionInFlatTree& end) {
  return TextIteratorInFlatTree::RangeLength(
      start, end,
      TextIteratorBehavior::AllVisiblePositionsRangeLengthBehavior());
}
