InlineBoxPosition ComputeInlineBoxPosition(const PositionInFlatTree& position,
                                           TextAffinity affinity) {
  return ComputeInlineBoxPositionTemplate<EditingInFlatTreeStrategy>(position,
                                                                     affinity);
}
