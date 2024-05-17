base::Optional<MinMaxSize> NGColumnLayoutAlgorithm::ComputeMinMaxSize(
    const MinMaxSizeInput& input) const {
  NGConstraintSpace space = CreateConstraintSpaceForMinMax();
  NGFragmentGeometry fragment_geometry =
      CalculateInitialMinMaxFragmentGeometry(space, Node());
  NGBlockLayoutAlgorithm algorithm({Node(), fragment_geometry, space});
  MinMaxSizeInput child_input(input);
  child_input.size_type = NGMinMaxSizeType::kContentBoxSize;
  base::Optional<MinMaxSize> min_max_sizes =
      algorithm.ComputeMinMaxSize(child_input);
  DCHECK(min_max_sizes.has_value());
  MinMaxSize sizes = *min_max_sizes;

  if (!Style().HasAutoColumnWidth()) {
    sizes.min_size =
        std::max(sizes.min_size, LayoutUnit(Style().ColumnWidth()));
    sizes.max_size = std::max(sizes.max_size, sizes.min_size);
  }

  int column_count = Style().ColumnCount();
  DCHECK_GE(column_count, 1);
  sizes.min_size *= column_count;
  sizes.max_size *= column_count;
  LayoutUnit column_gap = ResolveUsedColumnGap(LayoutUnit(), Style());
  sizes += column_gap * (column_count - 1);

  if (input.size_type == NGMinMaxSizeType::kBorderBoxSize) {
    sizes += border_scrollbar_padding_.InlineSum();
  }

  return sizes;
}
