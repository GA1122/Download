    UpdateRepeatingTableSectionPaintOffsetAdjustment() {
  if (!context_.is_repeating_table_section)
    return;

  if (object_.IsTableSection()) {
    if (ToLayoutTableSection(object_).IsRepeatingHeaderGroup())
      UpdateRepeatingTableHeaderPaintOffsetAdjustment();
    else if (ToLayoutTableSection(object_).IsRepeatingFooterGroup())
      UpdateRepeatingTableFooterPaintOffsetAdjustment();
  } else if (!context_.painting_layer->EnclosingPaginationLayer()) {
    for (auto& fragment_context : context_.fragments) {
      fragment_context.repeating_paint_offset_adjustment = LayoutSize();
    }
  }

}
