void FragmentPaintPropertyTreeBuilder::UpdatePaintOffset() {
  const auto* enclosing_pagination_layer =
      full_context_.painting_layer->EnclosingPaginationLayer();
  if (enclosing_pagination_layer &&
      !context_.current.paint_offset_root->PaintingLayer()
           ->EnclosingPaginationLayer()) {
    LayoutPoint paint_offset =
        PaintOffsetInPaginationContainer(object_, *enclosing_pagination_layer);

    paint_offset.MoveBy(fragment_data_.PaginationOffset());
    paint_offset.Move(context_.repeating_paint_offset_adjustment);
    paint_offset.MoveBy(
        VisualOffsetFromPaintOffsetRoot(context_, enclosing_pagination_layer));

    paint_offset.MoveBy(
        context_.current.paint_offset_root->FirstFragment().PaintOffset());

    context_.current.paint_offset = paint_offset;

    return;
  }

  if (object_.IsFloating())
    context_.current.paint_offset = context_.paint_offset_for_float;

  if (object_.IsColumnSpanAll()) {
    context_.current.paint_offset =
        object_.Container()->FirstFragment().PaintOffset();
  }

  if (object_.IsBoxModelObject()) {
    const LayoutBoxModelObject& box_model_object =
        ToLayoutBoxModelObject(object_);
    switch (box_model_object.StyleRef().GetPosition()) {
      case EPosition::kStatic:
        break;
      case EPosition::kRelative:
        context_.current.paint_offset +=
            box_model_object.OffsetForInFlowPosition();
        break;
      case EPosition::kAbsolute: {
        DCHECK(full_context_.container_for_absolute_position ==
               box_model_object.Container());
        context_.current = context_.absolute_position;

        const auto* container = full_context_.container_for_absolute_position;
        if (container && container->IsLayoutInline()) {
          DCHECK(container->CanContainAbsolutePositionObjects());
          DCHECK(box_model_object.IsBox());
          context_.current.paint_offset +=
              ToLayoutInline(container)->OffsetForInFlowPositionedInline(
                  ToLayoutBox(box_model_object));
        }
        break;
      }
      case EPosition::kSticky:
        context_.current.paint_offset +=
            box_model_object.OffsetForInFlowPosition();
        break;
      case EPosition::kFixed: {
        DCHECK(full_context_.container_for_fixed_position ==
               box_model_object.Container());
        context_.current = context_.fixed_position;
        if (context_.fixed_position.fixed_position_children_fixed_to_root)
          context_.current.paint_offset_root = &box_model_object;

        const auto* container = full_context_.container_for_fixed_position;
        if (container && container->IsLayoutInline()) {
          DCHECK(container->CanContainFixedPositionObjects());
          DCHECK(box_model_object.IsBox());
          context_.current.paint_offset +=
              ToLayoutInline(container)->OffsetForInFlowPositionedInline(
                  ToLayoutBox(box_model_object));
        }
        break;
      }
      default:
        NOTREACHED();
    }
  }

  if (object_.IsBox()) {
    context_.current.paint_offset.MoveBy(
        ToLayoutBox(object_).PhysicalLocation());

    if (object_.IsTableCell()) {
      LayoutObject* parent_row = object_.Parent();
      DCHECK(parent_row && parent_row->IsTableRow());
      context_.current.paint_offset.MoveBy(
          -ToLayoutBox(parent_row)->PhysicalLocation());
    }
  }

  context_.current.paint_offset.Move(
      context_.repeating_paint_offset_adjustment);
}
