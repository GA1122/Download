static LayoutRect BoundingBoxInPaginationContainer(
    const LayoutObject& object,
    const PaintLayer& enclosing_pagination_layer) {
  if (!object.IsBox() && !object.HasLayer()) {
    const LayoutBox& containining_block = *object.ContainingBlock();
    LayoutRect bounds_rect;
    if (!object.IsSVG()) {
      bounds_rect = object.LocalVisualRect();
      containining_block.FlipForWritingMode(bounds_rect);
    } else {
      bounds_rect = LayoutRect(SVGLayoutSupport::LocalVisualRect(object));
    }

    return MapLocalRectToAncestorLayer(containining_block, bounds_rect,
                                       enclosing_pagination_layer);
  }

  if (object.HasLayer() && !object.IsTableSection()) {
    return ToLayoutBoxModelObject(object).Layer()->PhysicalBoundingBox(
        &enclosing_pagination_layer);
  }

  const LayoutBox& box = ToLayoutBox(object);
  auto bounding_box = MapLocalRectToAncestorLayer(box, box.BorderBoxRect(),
                                                  enclosing_pagination_layer);

  if (!IsRepeatingTableSection(object))
    return bounding_box;

  const auto& section = ToLayoutTableSection(object);
  const auto& table = *section.Table();

  if (section.IsRepeatingHeaderGroup()) {
    if (const auto* bottom_section = table.BottomNonEmptySection()) {
      bounding_box.Unite(MapLocalRectToAncestorLayer(
          *bottom_section, bottom_section->BorderBoxRect(),
          enclosing_pagination_layer));
    }
    return bounding_box;
  }

  DCHECK(section.IsRepeatingFooterGroup());
  const auto* top_section = table.TopNonEmptySection();
  if (top_section) {
    bounding_box.Unite(MapLocalRectToAncestorLayer(*top_section,
                                                   top_section->BorderBoxRect(),
                                                   enclosing_pagination_layer));
    auto top_exclusion = table.RowOffsetFromRepeatingFooter();
    if (const auto* top_section = table.TopNonEmptySection()) {
      DCHECK(top_section != section);
      top_exclusion +=
          top_section->FirstRow()->LogicalHeight() + table.VBorderSpacing();
    }
    if (top_exclusion)
      top_exclusion -= 1;
    bounding_box.ShiftYEdgeTo(bounding_box.Y() + top_exclusion);
  }
  return bounding_box;
}
