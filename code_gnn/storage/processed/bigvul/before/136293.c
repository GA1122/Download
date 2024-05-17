bool PaintPropertyTreeBuilder::IsRepeatingInPagedMedia() const {
  return context_.is_repeating_fixed_position ||
         (context_.is_repeating_table_section &&
          !context_.painting_layer->EnclosingPaginationLayer());
}
