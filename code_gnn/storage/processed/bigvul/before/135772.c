static PositionInFlatTree ComputeStartFromEndForExtendForward(
    const PositionInFlatTree& end,
    TextGranularity granularity) {
  if (granularity == TextGranularity::kCharacter)
    return end;
  return ComputeStartRespectingGranularity(
      PreviousPositionOf(CreateVisiblePosition(end),
                         kCannotCrossEditingBoundary)
          .DeepEquivalent(),
      granularity);
}
