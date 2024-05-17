bool PaintPropertyTreeBuilder::UpdateFragments() {
  bool had_paint_properties = object_.FirstFragment().PaintProperties();
  bool needs_paint_properties =
      object_.StyleRef().ClipPath() || NeedsPaintOffsetTranslation(object_) ||
      NeedsTransform(object_) || NeedsClipPathClip(object_) ||
      NeedsEffect(object_) || NeedsTransformForNonRootSVG(object_) ||
      NeedsFilter(object_) || NeedsCssClip(object_) ||
      NeedsInnerBorderRadiusClip(object_) || NeedsOverflowClip(object_) ||
      NeedsPerspective(object_) || NeedsSVGLocalToBorderBoxTransform(object_) ||
      NeedsScrollOrScrollTranslation(object_);

  if (object_.IsFixedPositionObjectInPagedMedia()) {
    context_.is_repeating_fixed_position = true;
    CreateFragmentContextsForRepeatingFixedPosition();
  } else if (ObjectIsRepeatingTableSectionInPagedMedia()) {
    context_.is_repeating_table_section = true;
    CreateFragmentContextsForRepeatingTableSectionInPagedMedia();
  }

  if (IsRepeatingInPagedMedia()) {
    CreateFragmentDataForRepeatingInPagedMedia(needs_paint_properties);
  } else if (context_.painting_layer->ShouldFragmentCompositedBounds()) {
    CreateFragmentContextsInFlowThread(needs_paint_properties);
  } else {
    InitSingleFragmentFromParent(needs_paint_properties);
    UpdateCompositedLayerPaginationOffset();
    context_.is_repeating_fixed_position = false;
    context_.is_repeating_table_section = false;
  }

  if (object_.IsSVGHiddenContainer()) {
    context_.fragments.clear();
    context_.fragments.Grow(1);
    context_.has_svg_hidden_container_ancestor = true;
    PaintPropertyTreeBuilderFragmentContext& fragment_context =
        context_.fragments[0];

    fragment_context.current.paint_offset_root =
        fragment_context.absolute_position.paint_offset_root =
            fragment_context.fixed_position.paint_offset_root = &object_;

    object_.GetMutableForPainting().FirstFragment().ClearNextFragment();
  }

  if (object_.HasLayer()) {
    ToLayoutBoxModelObject(object_).Layer()->SetIsUnderSVGHiddenContainer(
        context_.has_svg_hidden_container_ancestor);
  }

  UpdateRepeatingTableSectionPaintOffsetAdjustment();

  return needs_paint_properties != had_paint_properties;
}
