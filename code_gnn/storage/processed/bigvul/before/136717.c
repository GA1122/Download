void FrameSelection::MoveRangeSelection(const IntPoint& base_point,
                                        const IntPoint& extent_point,
                                        TextGranularity granularity) {
  const VisiblePosition& base_position =
      VisiblePositionForContentsPoint(base_point, GetFrame());
  const VisiblePosition& extent_position =
      VisiblePositionForContentsPoint(extent_point, GetFrame());
  MoveRangeSelectionInternal(
      SelectionInDOMTree::Builder()
          .SetBaseAndExtentDeprecated(base_position.DeepEquivalent(),
                                      extent_position.DeepEquivalent())
          .SetAffinity(base_position.Affinity())
          .Build(),
      granularity);
}
