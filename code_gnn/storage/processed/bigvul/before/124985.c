void RenderFlexibleBox::applyStretchAlignmentToChild(RenderBox* child, LayoutUnit lineCrossAxisExtent)
{
    if (!isColumnFlow() && child->style()->logicalHeight().isAuto()) {
        if (!hasOrthogonalFlow(child)) {
            LayoutUnit heightBeforeStretching = needToStretchChildLogicalHeight(child) ? constrainedChildIntrinsicContentLogicalHeight(child) : child->logicalHeight();
            LayoutUnit stretchedLogicalHeight = heightBeforeStretching + availableAlignmentSpaceForChildBeforeStretching(lineCrossAxisExtent, child);
            ASSERT(!child->needsLayout());
            LayoutUnit desiredLogicalHeight = child->constrainLogicalHeightByMinMax(stretchedLogicalHeight, heightBeforeStretching - child->borderAndPaddingLogicalHeight());

            if (desiredLogicalHeight != child->logicalHeight()) {
                child->setOverrideLogicalContentHeight(desiredLogicalHeight - child->borderAndPaddingLogicalHeight());
                child->setLogicalHeight(0);
                child->forceChildLayout();
            }
        }
    } else if (isColumnFlow() && child->style()->logicalWidth().isAuto()) {
        if (hasOrthogonalFlow(child)) {
            LayoutUnit childWidth = std::max<LayoutUnit>(0, lineCrossAxisExtent - crossAxisMarginExtentForChild(child));
            childWidth = child->constrainLogicalWidthByMinMax(childWidth, childWidth, this);

            if (childWidth != child->logicalWidth()) {
                child->setOverrideLogicalContentWidth(childWidth - child->borderAndPaddingLogicalWidth());
                child->forceChildLayout();
            }
        }
    }
}
