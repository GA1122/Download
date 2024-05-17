int RenderFlexibleBox::firstLineBoxBaseline() const
{
    if (isWritingModeRoot() || m_numberOfInFlowChildrenOnFirstLine <= 0)
        return -1;
    RenderBox* baselineChild = 0;
    int childNumber = 0;
    for (RenderBox* child = m_orderIterator.first(); child; child = m_orderIterator.next()) {
        if (child->isOutOfFlowPositioned())
            continue;
        if (alignmentForChild(child) == ItemPositionBaseline && !hasAutoMarginsInCrossAxis(child)) {
            baselineChild = child;
            break;
        }
        if (!baselineChild)
            baselineChild = child;

        ++childNumber;
        if (childNumber == m_numberOfInFlowChildrenOnFirstLine)
            break;
    }

    if (!baselineChild)
        return -1;

    if (!isColumnFlow() && hasOrthogonalFlow(baselineChild))
        return crossAxisExtentForChild(baselineChild) + baselineChild->logicalTop();
    if (isColumnFlow() && !hasOrthogonalFlow(baselineChild))
        return mainAxisExtentForChild(baselineChild) + baselineChild->logicalTop();

    int baseline = baselineChild->firstLineBoxBaseline();
    if (baseline == -1) {
        LineDirectionMode direction = isHorizontalWritingMode() ? HorizontalLine : VerticalLine;
        return synthesizedBaselineFromContentBox(baselineChild, direction) + baselineChild->logicalTop();
    }

    return baseline + baselineChild->logicalTop();
}
