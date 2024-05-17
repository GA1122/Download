static Corner CornerToAnchor(const ScrollableArea* scroller) {
  const ComputedStyle* style = ScrollerLayoutBox(scroller)->Style();
  if (style->IsFlippedBlocksWritingMode())
    return Corner::kTopRight;
  return Corner::kTopLeft;
}
