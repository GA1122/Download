InlineBoxPosition ComputeInlineBoxPosition(const Position& position,
                                           TextAffinity affinity) {
  return ComputeInlineBoxPositionTemplate<EditingStrategy>(position, affinity);
}
