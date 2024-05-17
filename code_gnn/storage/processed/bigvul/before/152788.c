LayoutUnit NGColumnLayoutAlgorithm::ConstrainColumnBlockSize(
    LayoutUnit size) const {

  LayoutUnit extra = border_scrollbar_padding_.BlockSum();
  size += extra;

  const ComputedStyle& style = Style();
  LayoutUnit max = ResolveMaxBlockLength(
      ConstraintSpace(), style, border_padding_, style.LogicalMaxHeight(), size,
      LengthResolvePhase::kLayout);
  LayoutUnit extent = ResolveMainBlockLength(
      ConstraintSpace(), style, border_padding_, style.LogicalHeight(), size,
      LengthResolvePhase::kLayout);
  if (extent != kIndefiniteSize) {
    max = std::min(max, extent);
  }

  size = std::min(size, max);
  return size - extra;
}
