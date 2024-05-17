bool RenderFlexibleBox::updateAutoMarginsInCrossAxis(RenderBox* child, LayoutUnit availableAlignmentSpace)
{
    ASSERT(!child->isOutOfFlowPositioned());
    ASSERT(availableAlignmentSpace >= 0);

    bool isHorizontal = isHorizontalFlow();
    Length topOrLeft = isHorizontal ? child->style()->marginTop() : child->style()->marginLeft();
    Length bottomOrRight = isHorizontal ? child->style()->marginBottom() : child->style()->marginRight();
    if (topOrLeft.isAuto() && bottomOrRight.isAuto()) {
        adjustAlignmentForChild(child, availableAlignmentSpace / 2);
        if (isHorizontal) {
            child->setMarginTop(availableAlignmentSpace / 2);
            child->setMarginBottom(availableAlignmentSpace / 2);
        } else {
            child->setMarginLeft(availableAlignmentSpace / 2);
            child->setMarginRight(availableAlignmentSpace / 2);
        }
        return true;
    }
    bool shouldAdjustTopOrLeft = true;
    if (isColumnFlow() && !child->style()->isLeftToRightDirection()) {
        shouldAdjustTopOrLeft = false;
    }
    if (!isColumnFlow() && child->style()->isFlippedBlocksWritingMode()) {
        shouldAdjustTopOrLeft = false;
    }

    if (topOrLeft.isAuto()) {
        if (shouldAdjustTopOrLeft)
            adjustAlignmentForChild(child, availableAlignmentSpace);

        if (isHorizontal)
            child->setMarginTop(availableAlignmentSpace);
        else
            child->setMarginLeft(availableAlignmentSpace);
        return true;
    }
    if (bottomOrRight.isAuto()) {
        if (!shouldAdjustTopOrLeft)
            adjustAlignmentForChild(child, availableAlignmentSpace);

        if (isHorizontal)
            child->setMarginBottom(availableAlignmentSpace);
        else
            child->setMarginRight(availableAlignmentSpace);
        return true;
    }
    return false;
}
