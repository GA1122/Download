LogicalSize NGColumnLayoutAlgorithm::CalculateColumnSize(
    const LogicalSize& content_box_size) {
  LogicalSize column_size = content_box_size;
  DCHECK_GE(column_size.inline_size, LayoutUnit());
  column_size.inline_size =
      ResolveUsedColumnInlineSize(column_size.inline_size, Style());

  if (NeedsColumnBalancing(column_size.block_size, Style())) {
    int used_count =
        ResolveUsedColumnCount(content_box_size.inline_size, Style());
    column_size.block_size =
        CalculateBalancedColumnBlockSize(column_size, used_count);
  }

  return column_size;
}
