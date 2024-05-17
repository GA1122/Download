InlineBoxPosition ComputeInlineBoxPosition(const PositionInFlatTree& position,
                                           TextAffinity affinity,
                                           TextDirection primary_direction) {
  return ComputeInlineBoxPositionTemplate<EditingInFlatTreeStrategy>(
      position, affinity, primary_direction);
}
