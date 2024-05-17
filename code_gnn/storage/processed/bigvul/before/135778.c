static SelectionInFlatTree ExtendSelectionAsNonDirectional(
    const PositionInFlatTree& position,
    const VisibleSelectionInFlatTree& selection,
    TextGranularity granularity) {
  DCHECK(!selection.IsNone());
  DCHECK(position.IsNotNull());
  const PositionInFlatTree& start = selection.Start();
  const PositionInFlatTree& end = selection.End();
  if (position < start) {
    return SelectionInFlatTree::Builder()
        .SetBaseAndExtent(
            end, ComputeStartRespectingGranularity(
                     PositionInFlatTreeWithAffinity(position), granularity))
        .Build();
  }
  if (end < position) {
    return SelectionInFlatTree::Builder()
        .SetBaseAndExtent(
            start,
            ComputeEndRespectingGranularity(
                start, PositionInFlatTreeWithAffinity(position), granularity))
        .Build();
  }
  const int distance_to_start = TextDistance(start, position);
  const int distance_to_end = TextDistance(position, end);
  if (distance_to_start <= distance_to_end) {
    return SelectionInFlatTree::Builder()
        .SetBaseAndExtent(
            end, ComputeStartRespectingGranularity(
                     PositionInFlatTreeWithAffinity(position), granularity))
        .Build();
  }
  return SelectionInFlatTree::Builder()
      .SetBaseAndExtent(
          start,
          ComputeEndRespectingGranularity(
              start, PositionInFlatTreeWithAffinity(position), granularity))
      .Build();
}
