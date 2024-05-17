bool NGFlexLayoutAlgorithm::MainAxisIsInlineAxis(NGBlockNode child) {
  return child.Style().IsHorizontalWritingMode() ==
         FlexLayoutAlgorithm::IsHorizontalFlow(Style());
}
