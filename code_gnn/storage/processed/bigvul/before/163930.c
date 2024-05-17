InlineBoxPosition ComputeInlineBoxPositionTemplate(
    const PositionTemplate<Strategy>& position,
    TextAffinity affinity) {
  return ComputeInlineBoxPositionTemplate<Strategy>(
      position, affinity, PrimaryDirectionOf(*position.AnchorNode()));
}
