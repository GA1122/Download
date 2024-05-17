LayoutUnit NGFlexLayoutAlgorithm::MainAxisContentExtent(
    LayoutUnit sum_hypothetical_main_size) {
  if (Style().IsColumnFlexDirection()) {
    return ComputeBlockSizeForFragment(
               ConstraintSpace(), Style(),
               sum_hypothetical_main_size + (borders_ + padding_).BlockSum()) -
           border_scrollbar_padding_.BlockSum();
  }
  return content_box_size_.inline_size;
}
