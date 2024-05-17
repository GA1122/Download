LayoutUnit NGColumnLayoutAlgorithm::StretchColumnBlockSize(
    LayoutUnit minimal_space_shortage,
    LayoutUnit current_column_size,
    LayoutUnit container_content_box_block_size) const {
  if (!NeedsColumnBalancing(container_content_box_block_size, Style()))
    return current_column_size;
  LayoutUnit length = current_column_size + minimal_space_shortage;
  return ConstrainColumnBlockSize(length);
}
