bool RenderFlexibleBox::computeNextFlexLine(OrderedFlexItemList& orderedChildren, LayoutUnit& sumFlexBaseSize, double& totalFlexGrow, double& totalWeightedFlexShrink, LayoutUnit& sumHypotheticalMainSize, bool& hasInfiniteLineLength, bool relayoutChildren)
{
    orderedChildren.clear();
    sumFlexBaseSize = 0;
    totalFlexGrow = totalWeightedFlexShrink = 0;
    sumHypotheticalMainSize = 0;

    if (!m_orderIterator.currentChild())
        return false;

    LayoutUnit lineBreakLength = mainAxisContentExtent(LayoutUnit::max());
    hasInfiniteLineLength = lineBreakLength == LayoutUnit::max();

    bool lineHasInFlowItem = false;

    for (RenderBox* child = m_orderIterator.currentChild(); child; child = m_orderIterator.next()) {
        if (child->isOutOfFlowPositioned()) {
            orderedChildren.append(child);
            continue;
        }

        LayoutUnit childMainAxisExtent = preferredMainAxisContentExtentForChild(child, hasInfiniteLineLength, relayoutChildren);
        LayoutUnit childMainAxisMarginBorderPadding = mainAxisBorderAndPaddingExtentForChild(child)
            + (isHorizontalFlow() ? child->marginWidth() : child->marginHeight());
        LayoutUnit childFlexBaseSize = childMainAxisExtent + childMainAxisMarginBorderPadding;

        LayoutUnit childMinMaxAppliedMainAxisExtent = adjustChildSizeForMinAndMax(child, childMainAxisExtent);
        LayoutUnit childHypotheticalMainSize = childMinMaxAppliedMainAxisExtent + childMainAxisMarginBorderPadding;

        if (isMultiline() && sumHypotheticalMainSize + childHypotheticalMainSize > lineBreakLength && lineHasInFlowItem)
            break;
        orderedChildren.append(child);
        lineHasInFlowItem  = true;
        sumFlexBaseSize += childFlexBaseSize;
        totalFlexGrow += child->style()->flexGrow();
        totalWeightedFlexShrink += child->style()->flexShrink() * childMainAxisExtent;
        sumHypotheticalMainSize += childHypotheticalMainSize;
    }
    return true;
}
