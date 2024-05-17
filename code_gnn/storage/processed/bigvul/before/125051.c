void RenderFlexibleBox::repositionLogicalHeightDependentFlexItems(Vector<LineContext>& lineContexts)
{
    LayoutUnit crossAxisStartEdge = lineContexts.isEmpty() ? LayoutUnit() : lineContexts[0].crossAxisOffset;
    alignFlexLines(lineContexts);

    if (!isMultiline() && lineContexts.size() == 1)
        lineContexts[0].crossAxisExtent = crossAxisContentExtent();
    alignChildren(lineContexts);

    if (style()->flexWrap() == FlexWrapReverse)
        flipForWrapReverse(lineContexts, crossAxisStartEdge);

    flipForRightToLeftColumn();
}
