void PaintPropertyTreeBuilder::CreateFragmentContextsInFlowThread(
    bool needs_paint_properties) {
  PaintLayer* paint_layer = context_.painting_layer;
  PaintLayer* enclosing_pagination_layer =
      paint_layer->EnclosingPaginationLayer();

  const auto& flow_thread =
      ToLayoutFlowThread(enclosing_pagination_layer->GetLayoutObject());
  LayoutRect object_bounding_box_in_flow_thread;
  if (context_.is_repeating_table_section) {
    object_bounding_box_in_flow_thread =
        context_.repeating_table_section_bounding_box;
  } else {
    object_bounding_box_in_flow_thread =
        BoundingBoxInPaginationContainer(object_, *enclosing_pagination_layer);
    if (IsRepeatingTableSection(object_)) {
      context_.is_repeating_table_section = true;
      context_.repeating_table_section_bounding_box =
          object_bounding_box_in_flow_thread;
    }
  }

  FragmentData* current_fragment_data = nullptr;
  FragmentainerIterator iterator(flow_thread,
                                 object_bounding_box_in_flow_thread);
  bool fragments_changed = false;
  Vector<PaintPropertyTreeBuilderFragmentContext, 1> new_fragment_contexts;
  for (; !iterator.AtEnd(); iterator.Advance()) {
    auto pagination_offset = ToLayoutPoint(iterator.PaginationOffset());
    auto logical_top_in_flow_thread =
        iterator.FragmentainerLogicalTopInFlowThread();
    base::Optional<LayoutRect> fragment_clip;

    if (object_.HasLayer()) {
      fragment_clip = iterator.ClipRectInFlowThread();
      fragment_clip->MoveBy(pagination_offset);
      {
        DCHECK(context_.fragments[0].current.paint_offset_root);
        LayoutPoint pagination_visual_offset = VisualOffsetFromPaintOffsetRoot(
            context_.fragments[0], enclosing_pagination_layer);
        pagination_visual_offset.MoveBy(
            context_.fragments[0]
                .current.paint_offset_root->FirstFragment()
                .PaintOffset());
        fragment_clip->MoveBy(pagination_visual_offset);
      }
    }

    new_fragment_contexts.push_back(
        ContextForFragment(fragment_clip, logical_top_in_flow_thread));

    base::Optional<LayoutUnit> old_logical_top_in_flow_thread;
    if (current_fragment_data) {
      if (const auto* old_fragment = current_fragment_data->NextFragment())
        old_logical_top_in_flow_thread = old_fragment->LogicalTopInFlowThread();
      current_fragment_data = &current_fragment_data->EnsureNextFragment();
    } else {
      current_fragment_data = &object_.GetMutableForPainting().FirstFragment();
      old_logical_top_in_flow_thread =
          current_fragment_data->LogicalTopInFlowThread();
    }

    if (!old_logical_top_in_flow_thread ||
        *old_logical_top_in_flow_thread != logical_top_in_flow_thread)
      fragments_changed = true;

    InitFragmentPaintProperties(
        *current_fragment_data,
        needs_paint_properties || new_fragment_contexts.back().fragment_clip,
        pagination_offset, logical_top_in_flow_thread);
  }

  if (!current_fragment_data) {
    InitSingleFragmentFromParent(needs_paint_properties);
  } else {
    if (current_fragment_data->NextFragment())
      fragments_changed = true;
    current_fragment_data->ClearNextFragment();
    context_.fragments = std::move(new_fragment_contexts);
  }

  if (fragments_changed)
    object_.GetMutableForPainting().SetSubtreeNeedsPaintPropertyUpdate();
}
