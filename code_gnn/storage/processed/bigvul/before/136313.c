bool PaintPropertyTreeBuilder::ObjectTypeMightNeedPaintProperties() const {
  return object_.IsBoxModelObject() || object_.IsSVG() ||
         context_.painting_layer->EnclosingPaginationLayer() ||
         context_.is_repeating_table_section ||
         context_.is_repeating_fixed_position;
}
