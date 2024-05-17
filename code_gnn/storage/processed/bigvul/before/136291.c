void PaintPropertyTreeBuilder::InitFragmentPaintProperties(
    FragmentData& fragment,
    bool needs_paint_properties,
    const LayoutPoint& pagination_offset,
    LayoutUnit logical_top_in_flow_thread) {
  if (needs_paint_properties) {
    fragment.EnsurePaintProperties();
  } else if (fragment.PaintProperties()) {
    context_.force_subtree_update = true;
    fragment.ClearPaintProperties();
  }
  fragment.SetPaginationOffset(pagination_offset);
  fragment.SetLogicalTopInFlowThread(logical_top_in_flow_thread);
}
