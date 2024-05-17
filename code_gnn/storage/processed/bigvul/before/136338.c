    UpdateRepeatingTableFooterPaintOffsetAdjustment() {
  const auto& section = ToLayoutTableSection(object_);
  DCHECK(section.IsRepeatingFooterGroup());

  LayoutUnit fragment_height;
  LayoutUnit original_offset_in_flow_thread =
      context_.repeating_table_section_bounding_box.MaxY() -
      section.LogicalHeight();
  LayoutUnit original_offset_in_fragment;
  const LayoutFlowThread* flow_thread = nullptr;
  if (const auto* pagination_layer =
          context_.painting_layer->EnclosingPaginationLayer()) {
    flow_thread = &ToLayoutFlowThread(pagination_layer->GetLayoutObject());
    if (!flow_thread->IsPageLogicalHeightKnown())
      return;

    fragment_height =
        flow_thread->PageLogicalHeightForOffset(original_offset_in_flow_thread);
    original_offset_in_fragment =
        fragment_height - flow_thread->PageRemainingLogicalHeightForOffset(
                              original_offset_in_flow_thread,
                              LayoutBox::kAssociateWithLatterPage);
  } else {
    fragment_height = object_.View()->PageLogicalHeight();
    original_offset_in_fragment =
        IntMod(original_offset_in_flow_thread, fragment_height);
  }

  const auto& table = *section.Table();
  auto repeating_offset_in_fragment = fragment_height -
                                      table.RowOffsetFromRepeatingFooter() -
                                      table.VBorderSpacing();
  if (table.ShouldCollapseBorders())
    repeating_offset_in_fragment -= table.BorderBottom();

  auto adjustment = repeating_offset_in_fragment - original_offset_in_fragment;

  auto fragment_offset_in_flow_thread =
      original_offset_in_flow_thread - original_offset_in_fragment;
  for (auto i = context_.fragments.size(); i > 0; --i) {
    auto& fragment_context = context_.fragments[i - 1];
    fragment_context.repeating_paint_offset_adjustment = LayoutSize();
    if (i != context_.fragments.size())
      fragment_context.repeating_paint_offset_adjustment.SetHeight(adjustment);

    adjustment -= fragment_height;
    fragment_offset_in_flow_thread -= fragment_height;

    if (flow_thread) {
      fragment_height = flow_thread->PageLogicalHeightForOffset(
          fragment_offset_in_flow_thread);
    }
  }
}
