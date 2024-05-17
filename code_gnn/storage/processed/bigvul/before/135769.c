static SelectionInFlatTree AdjustEndpointsAtBidiBoundary(
    const VisiblePositionInFlatTree& visible_base,
    const VisiblePositionInFlatTree& visible_extent) {
  DCHECK(visible_base.IsValid());
  DCHECK(visible_extent.IsValid());

  RenderedPosition base(visible_base);
  RenderedPosition extent(visible_extent);

  const SelectionInFlatTree& unchanged_selection =
      SelectionInFlatTree::Builder()
          .SetBaseAndExtent(visible_base.DeepEquivalent(),
                            visible_extent.DeepEquivalent())
          .Build();

  if (base.IsNull() || extent.IsNull() || base.IsEquivalent(extent))
    return unchanged_selection;

  if (base.AtLeftBoundaryOfBidiRun()) {
    if (!extent.AtRightBoundaryOfBidiRun(base.BidiLevelOnRight()) &&
        base.IsEquivalent(
            extent.LeftBoundaryOfBidiRun(base.BidiLevelOnRight()))) {
      return SelectionInFlatTree::Builder()
          .SetBaseAndExtent(
              CreateVisiblePosition(
                  ToPositionInFlatTree(base.PositionAtLeftBoundaryOfBiDiRun()))
                  .DeepEquivalent(),
              visible_extent.DeepEquivalent())
          .Build();
    }
    return unchanged_selection;
  }

  if (base.AtRightBoundaryOfBidiRun()) {
    if (!extent.AtLeftBoundaryOfBidiRun(base.BidiLevelOnLeft()) &&
        base.IsEquivalent(
            extent.RightBoundaryOfBidiRun(base.BidiLevelOnLeft()))) {
      return SelectionInFlatTree::Builder()
          .SetBaseAndExtent(
              CreateVisiblePosition(
                  ToPositionInFlatTree(base.PositionAtRightBoundaryOfBiDiRun()))
                  .DeepEquivalent(),
              visible_extent.DeepEquivalent())
          .Build();
    }
    return unchanged_selection;
  }

  if (extent.AtLeftBoundaryOfBidiRun() &&
      extent.IsEquivalent(
          base.LeftBoundaryOfBidiRun(extent.BidiLevelOnRight()))) {
    return SelectionInFlatTree::Builder()
        .SetBaseAndExtent(
            visible_base.DeepEquivalent(),
            CreateVisiblePosition(
                ToPositionInFlatTree(extent.PositionAtLeftBoundaryOfBiDiRun()))
                .DeepEquivalent())
        .Build();
  }

  if (extent.AtRightBoundaryOfBidiRun() &&
      extent.IsEquivalent(
          base.RightBoundaryOfBidiRun(extent.BidiLevelOnLeft()))) {
    return SelectionInFlatTree::Builder()
        .SetBaseAndExtent(
            visible_base.DeepEquivalent(),
            CreateVisiblePosition(
                ToPositionInFlatTree(extent.PositionAtRightBoundaryOfBiDiRun()))
                .DeepEquivalent())
        .Build();
  }
  return unchanged_selection;
}
