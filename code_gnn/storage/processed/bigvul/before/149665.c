static LayoutRect RelativeBounds(const LayoutObject* layout_object,
                                 const ScrollableArea* scroller) {
  PhysicalRect local_bounds;
  if (layout_object->IsBox()) {
    local_bounds = ToLayoutBox(layout_object)->PhysicalBorderBoxRect();
    if (!layout_object->HasOverflowClip()) {
      LayoutUnit max_y =
          std::max(local_bounds.Bottom(),
                   ToLayoutBox(layout_object)->LayoutOverflowRect().MaxY());
      auto* layout_block_flow = DynamicTo<LayoutBlockFlow>(layout_object);
      if (layout_block_flow && layout_block_flow->ContainsFloats()) {
        max_y = std::max(max_y, layout_block_flow->LowestFloatLogicalBottom());
      }
      local_bounds.ShiftBottomEdgeTo(max_y);
    }
  } else if (layout_object->IsText()) {
    const auto* text = ToLayoutText(layout_object);
    local_bounds.Unite(text->PhysicalLinesBoundingBox());
  } else {
    NOTREACHED();
  }

  LayoutRect relative_bounds = LayoutRect(
      scroller
          ->LocalToVisibleContentQuad(FloatRect(local_bounds), layout_object)
          .BoundingBox());

  return relative_bounds;
}
